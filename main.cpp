#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cassert>
#include "to-9km-and-beyond/Airplane.h"
#include "to-9km-and-beyond/AtmosphereProperties.h"
#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/Airfoil.h"
#include "to-9km-and-beyond/Nacelle.h"
#include "to-9km-and-beyond/Fuselage.h"
#include "to-9km-and-beyond/CF34_3B1.h"
#include "to-9km-and-beyond/kadenMath.h"

using namespace std;
using namespace atmosphere_properties;
using namespace airplane;
using namespace kaden_math;



/*
	To Do List:

	- In CF34_3B1 class:
		- Need to digitalize my plots
		- Need to write the fuel consumption / Thrust and mach number curve stuff into my engine class
		- Make it a proper derived class

	- Done Refactoring:
		- Wing
			- Well might need to add dynamic memory capabilities still...
		- AtmosphereProperties
		-

*/






struct wingOptimizerResults {
	// The best wing is in index 0
	// wing at wingVector[0] has a timeToClimb of timeToClimbVector[0]
	vector<Wing> wingVector;
	vector<double> climbTimeVector;
};


void sortWingsByClimbTime(vector<Wing>& wings, vector<double>& climbTimes);
wingOptimizerResults spanOptimizer(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage, double inFuelWeight, double inPayLoadWeight);
void printUsefulCharacteristics(Wing& inWing, Airplane& inAirplane);







// Useful main
int main() {
	// Aifoil :)
	Airfoil NACA2412("2412", -.0349);

	// Givens:
	Wing VT(NACA2412, 99.68, 86.68, 86.68, 40, 0);										// Set in Stone.. Altho not really needed bc ignoring contributions
	Wing HT(NACA2412, 239.67, 23.55, 72.45, 32.04, 0);									// Set in Stone
	double givenAirplaneWeight = 23000 * 2.20462;										// lbms, this it the HT + VT + Fuselage + startingFuel + Payload
	double startingFuelWeight = 1000 * 2.20462;											// lbm
	double payLoadWeight = 1000 * 2.20462;												// lbm
	givenAirplaneWeight = givenAirplaneWeight - startingFuelWeight - payLoadWeight - (2 * 1670);     // 1670 lbms is CF34_3B1
	double fuselageWeight = .95 * givenAirplaneWeight;									// lbms... we will also store the VT and HT weight in this
	double nacelleWeight = .025 * givenAirplaneWeight;									// just said each one is 2.5% of fuselage+VT+HT
	double fuselageLength = 216;														// inches
	double fuselageWettedArea = 811.55 * 12;                                            // inches
	Nacelle nacelle(nacelleWeight);
	Fuselage fuselage(fuselageWeight, fuselageLength, fuselageWettedArea);
	CF34_3B1 CF34_3B1;

	// Main Wing Stuff, this is what will be optimized
	double mainSpan = 12 * 12;
	double mainRootChord = 186.7;
	double mainTipChord = 74.7;
	double mainSweepAngle = 20;
	Wing mainWing(NACA2412, mainSpan, mainTipChord, mainRootChord, mainSweepAngle);      // mainWing weight will be calculated by airplane

	// Airplane we have with everything
	Airplane airplane(mainWing, HT, VT, CF34_3B1, nacelle, fuselage, startingFuelWeight, payLoadWeight);

	double startHeight = 0;
	double takeOffEndHeight = 500;



	/*
		printUsefulCharacteristics(mainWing, airplane);
		cout << airplane.calcBestTimeTo9km(startHeight, takeOffEndHeight) / 60 << " mins" << endl;
		cout << endl << endl << endl;
	*/


	wingOptimizerResults results;
	results = spanOptimizer(mainWing, HT, VT, CF34_3B1, nacelle, fuselage, startingFuelWeight, payLoadWeight);

	for (int i = 0; i < results.wingVector.size(); i++) {
		cout << "Time: " << results.climbTimeVector[i] / 60 << " mins";
		cout << " Wing Span: " << results.wingVector[i].getSpan() << " ft" << endl;
		cout << endl << endl;
	}

	return 0;
}








// Optimizer Stuff
	/*
		Approach:
		0) Fix Airfoil
		1) Fix ct and cr to reasonable values and optimize span
		2) Fix span at the top 3 best values and optimize ct and cr (use a nester loop for this)
		3) Fix all the previous variables and optimize sweep angle
		4) Re-run from start with new min and max variables
			- like take top 3 spans from span optimizer.. then optimize other vars... then run run optimizer for span..


		Notes:
			- Will have to re-run all of the results once I add:
				- Full plot digitizations (more accuracy)
				- Weight loss accounted for with engine and time

		After:
			- Would be nice to have data plotted
				- gamma vs time
				- height vs time
				- excess power vs time
				- max possible excess power - excess power vs time
				- AoA vs time
				- Other stuff :)
	*/












wingOptimizerResults spanOptimizer(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage, double inFuelWeight, double inPayLoadWeight) {
	wingOptimizerResults returnStruct;
	vector<Wing> wingVector;
	vector<double> climbTimeVector;

	double MIN_TOTAL_WEIGHT = 25000 * 2.205;     // Need airplane to be at least 25000 lbs
	double START_HEIGHT = 0;
	double START_VELOCITY = 0;
	double TAKE_OFF_END_HEIGHT = 500;

	double SPAN_MIN = 12 * 12;			// Just saying span is 8 ft rn (prob should be higher)
	double SPAN_MAX = 265 * 12;         // ~265 is the max.. but will let optimizer try up to 350
	int NUM_STEPS = 25;


	double step = (SPAN_MAX - SPAN_MIN) / NUM_STEPS;

	for (int i = 0; i < NUM_STEPS; i++) {
		Wing newWing(inWing);					// Calls copy constructor (equivalent to newWing = inWing)
		newWing.setSpan(SPAN_MIN + (i * step));
		wingVector.push_back(newWing);
	}


	for (int i = 0; i < wingVector.size(); i++) {
		Airplane newAirplane(wingVector[i], inHT, inVT, inEngine, inNacelle, inFuselage, inFuelWeight, inPayLoadWeight);

		double totalWeight = newAirplane.getWeight();
		if (totalWeight < MIN_TOTAL_WEIGHT) {
			double weightNeeded = MIN_TOTAL_WEIGHT - totalWeight;
			newAirplane.setMainWingWeight(newAirplane.getMainWingWeight() + weightNeeded);
		}

		assert(newAirplane.getWeight() >= MIN_TOTAL_WEIGHT - .1);
		double climbTime = newAirplane.calcBestTimeTo9km(START_HEIGHT, TAKE_OFF_END_HEIGHT);
		climbTimeVector.push_back(climbTime);
	}

	sortWingsByClimbTime(wingVector, climbTimeVector);

	returnStruct.wingVector = wingVector;
	returnStruct.climbTimeVector = climbTimeVector;
	return returnStruct;
}















// Sorts wings by their corresponding climbTimes in ascending order
void sortWingsByClimbTime(vector<Wing>& wings, vector<double>& climbTimes) {

	vector<int> indices;
	for (int i = 0; i < wings.size(); i++) {
		indices.push_back(i);
	}


	// Sort indices based on climbTimes
	for (int i = 0; i < indices.size() - 1; i++) {

		int minIndex = i;
		for (int j = i + 1; j < indices.size(); j++) {
			if (climbTimes[indices[j]] < climbTimes[indices[minIndex]]) {
				minIndex = j;
			}
		}

		// Swap indices
		int temp = indices[i];
		indices[i] = indices[minIndex];
		indices[minIndex] = temp;
	}



	// Create sorted copies
	vector<Wing> sortedWings;
	vector<double> sortedTimes;

	for (int i = 0; i < indices.size(); i++) {
		int idx = indices[i];
		sortedWings.push_back(wings[idx]);
		sortedTimes.push_back(climbTimes[idx]);
	}


	// Replace original vectors with sorted versions
	// std::move essentially makes copying more efficient (why we used index method)
	wings = move(sortedWings);
	climbTimes = move(sortedTimes);
}










void printUsefulCharacteristics(Wing& inWing, Airplane& inAirplane) {
	cout << fixed << setprecision(5);
	cout << "Area (ft^2): " << inWing.getArea() << endl;
	cout << "Span (ft): " << inWing.getSpan() << endl;
	cout << "AR: " << inWing.getAspectRatio() << endl;
	cout << "e: " << inWing.getEllipticalEffic() << endl;
	cout << "MAC (ft) " << inWing.getMAC() << endl;
	cout << "Taper Ratio: " << inWing.getTaperRatio() << endl;
	cout << "Wing Weight (lbm): " << inWing.getWeight() << endl;
	cout << "Total Weight (lbm): " << inAirplane.getWeight() << endl;
	cout << "Total Weight (kg): " << inAirplane.getWeight() / 2.20462 << endl;
	return;
}









/*
		cout << fixed << setprecision(5);
		cout << "Area: " << mainWing.getArea() << endl;
		cout << "AR: " << mainWing.getAspectRatio() << endl;
		cout << "e: " << mainWing.getEllipticalEffic() << endl;
		cout << "Sweep: " << mainSweepAngle << endl;
		cout << "MAC: " << mainWing.getMAC() << endl;
		cout << "Taper Ratio: " << mainWing.getTaperRatio() << endl;
		cout << "Wing Weight: " << mainWing.getWeight() << endl;
		cout << "Total Weight lbm: " << airplane.getWeight() << endl;
		cout << "Total Weight in kg: " << airplane.getWeight() / 2.20462 << endl;
		cout << calcTimeTo9km(airplane, startHeight, takeOffEndHeight) << " mins" << endl;
		cout << endl << endl << endl;

*/