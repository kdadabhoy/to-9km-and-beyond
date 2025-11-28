

/*	
	********************************************************************

							to-9km-and-beyond 
							By: Kaden Dadabhoy

	********************************************************************

	If you have any questions about the code or anything else, just email me!

							Emails (either works):
							- kdadabhoy28@gmail.com
							- kdadabho@uci.edu

	********************************************************************

	HEAVILY recommended to read the README file on the Github!
		- https://github.com/kdadabhoy/to-9km-and-beyond
		- Read it on the website compared to the .txt file... 
		because of markdown / HTML stuff
			- Just scroll down below the files... and that text is the README :)


	- The Header file's don't have traditional header documentation 
		(sorry, it would take forever), so the READMEis the only place 
		where they are explained

		- Which is also nice, because it is all in one place and 
			formatted nicely, compared to green comments

		- It is also explained in a more "English approach"... whereas
		  header documentation is usually written with the assumption the user
		  is already very familar with the language :)

		- There are comments in implementation files for some of the functions...
			- These comments tend to give a general idea of the implementation...
			  instead of explaining it line by line

			- A lof of these comments are physics derivations that are used in the functions

			- The general consesus is that comments explaining code implementation
			  in a lot of detail, tend to make the code more confusing and less readable
			  than just having the code by itself.
			
			- Again, look at Github, for very brief and Enlgish explanation... then look at the code
			  so that you understand what it is attempting to do
				- The function names do describe what the function does very accurately in most cases.

	********************************************************************

	TLDR of Everything Below this Comment:

		- Read the Github README, then come back to this... to:

		1) See that the code actually compiles and produces what I claim it does
		2) See the implementation of the optimizer (at the bottom of this file)
		3) See the implementation of any other function
			- In the implementation file of that particular Class

			- *** You probably will want to look at Airplane.cpp first ***
				- *** Then look at DragCoeff.cpp ***
					- Basically every class is setup so that it can easily
					  use the DragCoeff functions

				- Also, basically every other class's purpose is to make
				the Airplane.cpp more readable, modular, etc
					- And the Airplane.cpp's purpose is to easily
					write the optimizer

	********************************************************************

	- The main() program, this program, is currently set up to run an optimizer
	for the wing span only. It is running the optimizer that uses the calcBestTimeTo9km() 
	function (difference between this function and the other approach explained somewhat
	below... but read README for more details)



	- Below the optimizer function there are other functions that you can uncomment,
	which may help you understand the program better. These functions are also explained on Github,
	and not to sound like a broken record, but it is unlikely to understand this project completely
	without reading the Github. These functions are mainly included, so that you can see that
	the code compiles. 


	- Below these other functions, is the optimizer's implementation.



	- All of this code relies HEAVILY on every other class created, as explained in the GitHub.
		- "The classes, which will be described in much more detail in the 
		"Classes and Assumptions" section, whole purpose is to make it easier, 
		among other things (readbility, modularity, etc), to code an optimizer..."
			- Directly quoted from main() function, Overview / Approach portion of README



	- All the assumptions made are in the Github README


*/







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






// Structs:
struct wingSpanOptimizerResults {
	// The best wing is stored at index 0
	// wing at wingVector[0] has a timeToClimb of timeToClimbVector[0]
	vector<double> wingSpanVector;      // ft
	vector<double> climbTimeVector;     // minutes
};




// Function Protypes (What functions are in this file... needed for compiler purposes)
void printUsefulCharacteristics(Wing& inWing, Airplane& inAirplane);
void sortWingsByClimbTime(vector<double>& wingSpans, vector<double>& climbTimes);
void spanOptimizerResultsToCSV(wingSpanOptimizerResults& results, string fileName);

wingSpanOptimizerResults spanOptimizer(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage,
	double inFuelWeight, double inPayLoadWeight, double minSpanFt, double maxSpanFt, int numSteps);

wingSpanOptimizerResults spanOptimizerApprox(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage,
	double inFuelWeight, double inPayLoadWeight, double minSpanFt, double maxSpanFt, int numSteps);








int main() {
	// Aifoil :)
	Airfoil NACA2412("2412", -.0349);

	// Givens:
	Wing VT(NACA2412, 99.68, 86.68, 86.68, 40, 0);				// Given, from Doc
	Wing HT(NACA2412, 239.67, 23.55, 72.45, 32.04, 0);			// Given, from Doc
	CF34_3B1 CF34_3B1;                                          // Given engine
	double givenAirplaneWeight = 23000 * 2.20462;				// lbms, this it the HT + VT + Fuselage + startingFuel + Payload
	double startingFuelWeight = 1000 * 2.20462;					// lbm
	double payLoadWeight = 1000 * 2.20462;						// lbm

	givenAirplaneWeight = givenAirplaneWeight					// lbm.. needed to allocate other weights
		- startingFuelWeight - payLoadWeight 
		- (2 * CF34_3B1.getWeight());     

	double fuselageWeight = .95 * givenAirplaneWeight;		   // lbms... we will also store the VT and HT weight in this
	double nacelleWeight = .025 * givenAirplaneWeight;		   // lbms.. we will just said each one is 2.5% of fuselage+VT+HT
	double fuselageLength = 621.80;							   // inches, from CAD
	double fuselageWettedArea = 811.55 * 12;                   // inches, from Doc
	double nacelleWettedArea = 130.07 * 12;                    // inches, from CAD
	double nacelleLength = 87.17;                              // inches, from CAD
	Nacelle nacelle(nacelleWeight, nacelleLength, nacelleWettedArea);                 
	Fuselage fuselage(fuselageWeight, fuselageLength, fuselageWettedArea); 


	// Main Wing Stuff (Just a random Wing that we initialize)
	double mainSpan = 60.0 * 12;    // inches
	double mainRootChord = 186.7;	// inches
	double mainTipChord = 74.7;     // inches
	double mainSweepAngle = 20;     // degrees
	Wing mainWing(NACA2412, mainSpan, mainTipChord, mainRootChord, mainSweepAngle); 

	Airplane airplane(mainWing, HT, VT, CF34_3B1, nacelle, fuselage, startingFuelWeight, payLoadWeight);







/*
		***** Span Optimizer (Adjusts velocity while climbing) ****

	- This function uses calcBestTimeTo9km() 
		- Which adjusts V to be at V_maxExcessPower (within an error of course)...
			- What the error is set to can drastically change the climb times...
			- Currently the error is set to a value that gives around the same results as the Approx Climb Method
		- Read Github documentation or go over to Airplane.cpp to understand it's implementation
	- The optimizer essentially runs this simulation for a new aircraft with different traits (defined by the range you give the optimizer)
	- The optimizer also sorts that data, and bla bla bla, but the optimizer code might make more sense, if you understand this snippet of code

*/

	// This is the range the optimizer runs on
	wingSpanOptimizerResults results;
	double minSpanSimulated = 15;     // Min span in ft
	double maxSpanSimulated = 150;    // Max span in ft
	int numberOfSimulationSteps = 50; // How many evenly spaced steps in the range the optimizer will simulate
									  // 50 Steps ~ 1 min, obv more will take more time... and depends on computer

	results = spanOptimizer(mainWing, HT, VT, CF34_3B1, nacelle, fuselage, startingFuelWeight, payLoadWeight, minSpanSimulated, maxSpanSimulated, numberOfSimulationSteps);


	// Produced the .csv for the sorted data.
	// Also displays the results in the command window
	spanOptimizerResultsToCSV(results, "SpanOptimizerData.csv");
	for (int i = 0; i < results.wingSpanVector.size(); i++) {
		cout << fixed << setprecision(5);
		cout << "Time: " << results.climbTimeVector[i] << " mins";
		cout << " Wing Span: " << results.wingSpanVector[i] << " ft" << endl;
		cout << endl;

	}












/*
		***** Span Optimizer (Does not adjust Velocity while climbing) ****
		
	- This function uses calcBestTimeTo9km() 
		- Which adjusts V to be at V_maxExcessPower (within an error of course)...
			- What the error is set to can drastically change the climb times...
			- Currently the error is set to a value that gives around the same results as the Approx Climb Method
		- Read Github documentation or go over to Airplane.cpp to understand it's implementation

	- The optimizer essentially runs this simulation for a new aircraft with different traits (defined by the range you give the optimizer)
	- The optimizer also sorts that data, and bla bla bla, but the optimizer code might make more sense, if you understand this snippet of code

*/




/*

	// This is the range the optimizer runs on
	wingSpanOptimizerResults results;
	double minSpanSimulated = 15;     // Min span in ft
	double maxSpanSimulated = 150;    // Max span in ft
	int numberOfSimulationSteps = 50; // How many evenly spaced steps in the range the optimizer will simulate
									  // 50 Steps ~ 1 min, obv more will take more time... and depends on computer

	results = spanOptimizerApprox(mainWing, HT, VT, CF34_3B1, nacelle, fuselage, startingFuelWeight, payLoadWeight, minSpanSimulated, maxSpanSimulated, numberOfSimulationSteps);

	// Produced the .csv for the sorted data.
	// Also displays the results in the command window
	spanOptimizerResultsToCSV(results, "SpanOptimizerData.csv");
	for (int i = 0; i < results.wingSpanVector.size(); i++) {
		cout << fixed << setprecision(5);
		cout << "Time: " << results.climbTimeVector[i] << " mins";
		cout << " Wing Span: " << results.wingSpanVector[i] << " ft" << endl;
		cout << endl;

	}

*/





















/*
	 ************** Other Functions that might be intriguing (Useful for seeing how the program works) ***********
	 
	 - Recommended to Look at: 
		- Power Curve Plot Function
		- 1 Aircraft (the one initialized at the top of the program) Simulation

	- Comment out the optimizer (can just // next to the "results = spanOptimizer..." line)
	- Uncomment the code part of the function you want to look at
		- Just remove the /* and the corresponding * /

	
*/












/*
		***** 1 Aircraft (the one initialized at the top of the program) Simulation ****
	
	- Useful for debugging purposes or to quickly see 1 aircraft
		- The aircraft that is simulated is the one intialized at the top of main
	- The optimizer essentially runs this simulation for a new aircraft with different traits (defined by the range you give the optimizer)
	- The optimizer also sorts that data, and bla bla bla, but the optimizer code might make more sense, if you understand this snippet of code

*/


/*
	double startHeight = 0;
	double takeOffEndHeight = 500;
	double totalWeight = airplane.getWeight();


	if (totalWeight < 25000 * 2.205) {
		double weightNeeded = 25000 * 2.205 - totalWeight;
		airplane.setMainWingWeight(airplane.getMainWingWeight() + weightNeeded);
	}


	printUsefulCharacteristics(mainWing, airplane);
	cout << endl << endl;

	cout << airplane.calcBestTimeTo9km(startHeight, takeOffEndHeight) / 60 << " mins" << endl;
	cout << airplane.getMTOW() - airplane.getWeight() << " Weight Lost after 9km" << endl;
	cout << endl << endl << endl;
*/


















/*
						***** Power Curve Plot Function ****

	- It will produce .csv data for the Airplane intialized above at the given height
	- The .csv will be saved in the to-9km-and-beyond folder
	- Just open the .csv and select the A, B and C columns and create a scatter plot, in order to
		the Power Curve (Drag Bucket) for the given airplane at the provided height
	- Make sure you close the .csv or rename the fileNamePowerCurve before re-running
	- Power Curves like this one are used to find the maxExcessPower, currentExcessPower, etc
		- See Github documentation for a more detailed explanation... this occurs in "Airplane.cpp"
*/

/*
	double heightPowerCurveFunction = 0;
	string fileNamePowerCurve = "PowerCurve.csv";
	airplane.getPowerCurveCSV(heightPowerCurveFunction, fileNamePowerCurve);
*/

















	return 0;
}





























// Uses calcBestTimeTo9km (the approach that adjusts velocity to = velocity for maxExcessPower)
wingSpanOptimizerResults spanOptimizer(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage, 
										double inFuelWeight, double inPayLoadWeight, double minSpanFt, double maxSpanFt, int numSteps) {

	// Adjustable Constants:
	double SPAN_MIN = minSpanFt * 12;			// No more breaking... unless abort
	double SPAN_MAX = maxSpanFt * 12;			// Run it high, then adjust (*12 to convert to inches)
	int NUM_STEPS = numSteps;				// NUM_STEPS = 100 takes about 1 min and 15s to run

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

		cout << "Simulating Span (ft): " << newSpan/12 << endl; // delete

		Airplane newAirplane(newWing, inHT, inVT, inEngine, inNacelle, inFuselage, inFuelWeight, inPayLoadWeight);

		double totalWeight = newAirplane.getWeight();
		if (totalWeight < MIN_TOTAL_WEIGHT) {
			double weightNeeded = MIN_TOTAL_WEIGHT - totalWeight;
			newAirplane.setMainWingWeight(newAirplane.getMainWingWeight() + weightNeeded);
		}

		assert(newAirplane.getWeight() >= MIN_TOTAL_WEIGHT - .1);

		double climbTime = newAirplane.calcBestTimeTo9km(START_HEIGHT, TAKE_OFF_END_HEIGHT);


		/* 
			// Unforunately the emperical equations used are not accurate enough
			// But this would check if our Plane can handle the stress required
			if (!newAirplane.isWingPossible()) {
			climbTime = 9e9; // Very large climb time
			}
		*/


		wingSpansVector.push_back(newSpan / INCHES_TO_FEET);     // stored in ft
		//climbTimeVector.push_back(climbTime);  // stored in seconds
		climbTimeVector.push_back(climbTime / SECONDS_TO_MINS);  // stored in mins
	}




	sortWingsByClimbTime(wingSpansVector, climbTimeVector);

	returnStruct.wingSpanVector = wingSpansVector;
	returnStruct.climbTimeVector = climbTimeVector;
	return returnStruct;
}























// Uses calcRoughApproxTimeTo9km (the approach that get's to original maxExcessPower velocity, and stays at it)
wingSpanOptimizerResults spanOptimizerApprox(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage,
	double inFuelWeight, double inPayLoadWeight, double minSpanFt, double maxSpanFt, int numSteps) {

	// Adjustable Constants:
	double SPAN_MIN = minSpanFt * 12;			// No more breaking... unless abort
	double SPAN_MAX = maxSpanFt * 12;			// Run it high, then adjust (*12 to convert to inches)
	int NUM_STEPS = numSteps;				// NUM_STEPS = 100 takes about 1 min and 15s to run

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

		cout << "Simulating Span (ft): " << newSpan / 12 << endl; // delete

		Airplane newAirplane(newWing, inHT, inVT, inEngine, inNacelle, inFuselage, inFuelWeight, inPayLoadWeight);

		double totalWeight = newAirplane.getWeight();
		if (totalWeight < MIN_TOTAL_WEIGHT) {
			double weightNeeded = MIN_TOTAL_WEIGHT - totalWeight;
			newAirplane.setMainWingWeight(newAirplane.getMainWingWeight() + weightNeeded);
		}

		assert(newAirplane.getWeight() >= MIN_TOTAL_WEIGHT - .1);

		double climbTime = newAirplane.calcRoughApproxTimeTo9km(START_HEIGHT, TAKE_OFF_END_HEIGHT);

		/* 
			// Unforunately the emperical equations used are not accurate enough
			// But this would check if our Plane can handle the stress required
			if (!newAirplane.isWingPossible()) {
			climbTime = 9e9; // Very large climb time
			}

		*/

		wingSpansVector.push_back(newSpan / INCHES_TO_FEET);     // stored in ft
		//climbTimeVector.push_back(climbTime);  // stored in seconds
		climbTimeVector.push_back(climbTime / SECONDS_TO_MINS);  // stored in mins
	}




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








































// Old implementaitons (ignore):




/*


	// Less passed parameters (Span_min, span_max, and numSteps set)
	wingSpanOptimizerResults spanOptimizer(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage,
		double inFuelWeight, double inPayLoadWeight) {

		// Adjustable Constants:
		double SPAN_MIN = 8 * 12;			// No more breaking... unless abort
		double SPAN_MAX = 100 * 12;			// Run it high, then adjust (*12 to convert to inches)
		int NUM_STEPS = 25;				// NUM_STEPS = 100 takes about 1 min and 15s to run

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

			cout << "Simulating Span (ft): " << newSpan / 12 << endl; // delete

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