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





struct wingSpanOptimizerResults {
	// The best wing is in index 0
	// wing at wingVector[0] has a timeToClimb of timeToClimbVector[0]
	vector<double> wingSpanVector;      // ft
	vector<double> climbTimeVector;     // minutes
};



void printUsefulCharacteristics(Wing& inWing, Airplane& inAirplane);
wingSpanOptimizerResults spanOptimizer(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage, double inFuelWeight, double inPayLoadWeight);
void sortWingsByClimbTime(vector<double>& wingSpans, vector<double>& climbTimes);
void spanOptimizerResultsToCSV(wingSpanOptimizerResults& results, string fileName);







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
	double fuselageLength = 621.80;														// inches
	double fuselageWettedArea = 811.55 * 12;                                            // inches
	Nacelle nacelle(nacelleWeight);
	Fuselage fuselage(fuselageWeight, fuselageLength, fuselageWettedArea); 
	CF34_3B1 CF34_3B1;

	// Main Wing Stuff, this is what will be optimized
	double mainSpan = 20 * 12;
	double mainRootChord = 186.7;
	double mainTipChord = 74.7;
	double mainSweepAngle = 20;
	Wing mainWing(NACA2412, mainSpan, mainTipChord, mainRootChord, mainSweepAngle);      // mainWing weight will be calculated by airplane

	Airplane airplane(mainWing, HT, VT, CF34_3B1, nacelle, fuselage, startingFuelWeight, payLoadWeight);

	

	/*
		// Debugging
		double startHeight = 0;
		double takeOffEndHeight = 500;
		double totalWeight = airplane.getWeight();
		if (totalWeight < 25000 * 2.205) {
			double weightNeeded = 25000 * 2.205 - totalWeight;
			airplane.setMainWingWeight(airplane.getMainWingWeight() + weightNeeded);
		}
		printUsefulCharacteristics(mainWing, airplane);
		cout << airplane.calcBestTimeTo9km(startHeight, takeOffEndHeight) / 60 << " mins" << endl;
		cout << endl << endl << endl;
		//airplane.getPowerCurveCSV(10000, "example.csv");
	*/







/*
		cout << "Feasibly? " << airplane.isWingPossible() << endl;

		double limitLift = airplane.calcLimitLift();
		double taperRatio = mainWing.getTaperRatio();
		double thicknessRatio = NACA2412.getThicknessRatio();

		double rootMomentfromOutside = (limitLift * mainWing.getSpan() * (1 + (2*mainWing.getTaperRatio())) / (6* (1 + mainWing.getTaperRatio())));

		double inertia = (.2/12) * (mainWing.getRootChord()) * (thicknessRatio * mainWing.getRootChord()) * (thicknessRatio * mainWing.getRootChord()) / 2;
		double C_CST = (thicknessRatio * mainWing.getRootChord()) / 2;
		double sigma = rootMomentfromOutside * C_CST / inertia;



		cout << "Limit Lift Outside " << limitLift << endl;
		cout << "Outside Root moment " << rootMomentfromOutside << endl;
		cout << "Outside C cst " << C_CST << endl;
		cout << "Outside inertia " << inertia << endl;
		cout << "Outside airfoil thick " << thicknessRatio << endl;
		cout << "Outside root stress (ksi) " << sigma * 0.006944 / 1000 << endl;

		cout << endl << endl;
		cout << "From Function Calls: " << endl;
		cout << "Root Moment (lbf*ft) " << airplane.calcRootLimitMoment() << endl;
		//cout << "C constant " << mainWing.calc_C_ForRootStress() << endl;
		cout << "Inertia " << mainWing.calcRootInertiaEstimate() << endl;
		cout << "Root Stress (ksi) " << airplane.calcRootLimitStress() << endl;
*/










	wingSpanOptimizerResults results;
	results = spanOptimizer(mainWing, HT, VT, CF34_3B1, nacelle, fuselage, startingFuelWeight, payLoadWeight);
	spanOptimizerResultsToCSV(results, "SpanOptimizerData.csv");

	for (int i = 0; i < results.wingSpanVector.size(); i++) {
		cout << fixed << setprecision(5);
		cout << "Time: " << results.climbTimeVector[i] << " mins";
		cout << " Wing Span: " << results.wingSpanVector[i] << " ft" << endl;
		cout << endl;

	}

	return 0;
}















// Efficient way
	// Eventually reWork to take in SPAN_MIN and SPAN_MAX and maybe NUM_STEPS
wingSpanOptimizerResults spanOptimizer(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage, double inFuelWeight, double inPayLoadWeight) {

	// Adjustable Constants:
	double SPAN_MIN = 12 * 12;			// Breaks below 12; Prob need to figure out a realistic min and set it here (will prob end up being the best wing)
	double SPAN_MAX = 100 * 12;			// Run it high, then adjust (*12 to convert to inches)
	int NUM_STEPS = 100;				// NUM_STEPS = 100 takes about 1 min and 15s to run

	double TAKE_OFF_END_HEIGHT = 500;   // Change to 0, 10, or 50ft when wanting to get best possible plane
	double START_HEIGHT = 0;            // Don't change
	double START_VELOCITY = 0;          // Don't Change

	// Other Intialization
	Wing newWing(inWing);						 // Calls copy constructor (equivalent to newWing = inWing)
	wingSpanOptimizerResults returnStruct;
	vector<double> wingSpansVector;
	vector<double> climbTimeVector;
	static const int INCHES_TO_FEET = 12;
	static const int SECONDS_TO_MINS = 60;
	double MIN_TOTAL_WEIGHT = 25000 * 2.205;     // Need airplane to be at least 25000 kgs

	double step = (SPAN_MAX - SPAN_MIN) / NUM_STEPS;

	for (int i = 0; i < NUM_STEPS; i++) {
		double newSpan = SPAN_MIN + (i * step);
		newWing.setSpan(newSpan);

		cout << "Span: " << newSpan << endl; // delete

		Airplane newAirplane(newWing, inHT, inVT, inEngine, inNacelle, inFuselage, inFuelWeight, inPayLoadWeight);

		double totalWeight = newAirplane.getWeight();
		if (totalWeight < MIN_TOTAL_WEIGHT) {
			double weightNeeded = MIN_TOTAL_WEIGHT - totalWeight;
			newAirplane.setMainWingWeight(newAirplane.getMainWingWeight() + weightNeeded);
		}

		assert(newAirplane.getWeight() >= MIN_TOTAL_WEIGHT - .1);

		double climbTime = newAirplane.calcBestTimeTo9km(START_HEIGHT, TAKE_OFF_END_HEIGHT);
		wingSpansVector.push_back(newSpan / INCHES_TO_FEET);     // stored in ft
		climbTimeVector.push_back(climbTime / SECONDS_TO_MINS);  // stored in mins
	}


	// need to add a realism check of the wing... in other words see if the wing can
	// 1) Tolerate the maximum bending moment (mainly this)
	// 2) Takeoff (has enough control authority to pitch up)

	sortWingsByClimbTime(wingSpansVector, climbTimeVector);

	returnStruct.wingSpanVector = wingSpansVector;
	returnStruct.climbTimeVector = climbTimeVector;
	return returnStruct;
}











// New sort function - Still could get more efficient using std::sort
void sortWingsByClimbTime(vector<double>& wingSpans, vector<double>& climbTimes) {
	assert(wingSpans.size() == climbTimes.size());

	vector<int> indices;
	for (int i = 0; i < wingSpans.size(); i++) {
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
	vector<double> sortedWingSpans;
	vector<double> sortedTimes;

	for (int i = 0; i < indices.size(); i++) {
		int idx = indices[i];
		sortedWingSpans.push_back(wingSpans[idx]);
		sortedTimes.push_back(climbTimes[idx]);
	}


	// Replace original vectors with sorted versions
	// std::move essentially makes copying more efficient (why we used index method)
	wingSpans = move(sortedWingSpans);
	climbTimes = move(sortedTimes);
}











// Could probably delete attemptNumber from this CSV... but it can be nice to have
void spanOptimizerResultsToCSV(wingSpanOptimizerResults& results, string fileName) {
	vector<double> attemptNumber;
	for (int i = 0; i < results.wingSpanVector.size(); i++) {
		attemptNumber.push_back(i + 1);
	}
	saveVectorsToCSV(attemptNumber, results.wingSpanVector, results.climbTimeVector, fileName);

	return;
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























// Optimizer Notes:
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














// Old implementaitons:



/*
	struct wingOptimizerResults {
		// The best wing is in index 0
		// wing at wingVector[0] has a timeToClimb of timeToClimbVector[0]
		vector<Wing> wingVector;      // Maybe don't use this to avoid passing so many object
		vector<double> climbTimeVector;
	};
*/


/*
// Way of doing it with creating a bunch of wings - inefficient
	wingOptimizerResults spanOptimizer(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage, double inFuelWeight, double inPayLoadWeight) {
		wingOptimizerResults returnStruct;
		vector<Wing> wingVector;
		vector<double> climbTimeVector;

		double MIN_TOTAL_WEIGHT = 25000 * 2.205;     // Need airplane to be at least 25000 kgs
		double START_HEIGHT = 0;
		double START_VELOCITY = 0;
		double TAKE_OFF_END_HEIGHT = 50;             // 50 ft is when we can just go straight until maxExcess Power... an assumption.. semi-pleasuible
													 // In reality we will climb and accelerate.. but since code is simplicity based this works

		double SPAN_MIN = 12 * 12;			// Just saying span is 8 ft rn (prob should be higher)
		double SPAN_MAX = 100 * 12;         // ~265 is the max.. but will let optimizer try up to 350
		int NUM_STEPS = 100;


		double step = (SPAN_MAX - SPAN_MIN) / NUM_STEPS;


		// Way of doing it with creating a bunch of wings - inefficient
		for (int i = 0; i < NUM_STEPS; i++) {
			Wing newWing(inWing);					// Calls copy constructor (equivalent to newWing = inWing)
			newWing.setSpan(SPAN_MIN + (i * step));
			wingVector.push_back(newWing);
		}


		Wing newWing(inWing);					// Calls copy constructor (equivalent to newWing = inWing)


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

*/





/*
	// Sort function for inefficinet way
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

*/