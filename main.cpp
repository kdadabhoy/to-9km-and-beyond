

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
	
	Brief Summary: This program is used to optimize a wing for a 
	subsonic aircraft (in the 25-35kg category) climbing to 9km. This 
	program accomplishes this by simulating a range of aircrafts first 
	by changing the span, then by changing the sweep angle, in order to 
	find the best wing.




	The main() program, this program, is currently set up to:
	1) Run an optimizer for the wing span 
	2) Run an optimizer for the wing sweep angle 
		- ONLY FOR THE BEST WING SPAN, from the wing span optimizer

	***Running this program, without any modifications, will run 25 simulations 
	for 25 different wing spans, in the range that the wing span optimizer is set for. 
	It will then take the best wing span from those simulations and then run 25 more 
	simulations with that wing span and different sweep angles ***


	********************************************************************

	HEAVILY recommended to read the README file on the Github!
		- https://github.com/kdadabhoy/to-9km-and-beyond
		- Read it on the website compared to the .md file...
		because of markdown / HTML stuff
			- Just scroll down below the files... and that text is the README :)


	********************************************************************

	SOME Assumptions and Disclosures:
		
		- Trapezodial Wing

		- Subsonic airplane (program will abort if airplane ever tries to fly above Mach .98)

		- Optimizers make no attempt to account for structural feasability
			- There are member functions in the Airplane Class, that attempt to deal with this,
			but those functions rely on unreliable emperical equations

		- "Optimizers" are currently set up in a manner that produces a .csv file
		of the sorted wing characteristic and it's associated time to climb to 9km
			- In a way, this isn't a traditional optimizer, but it still acts in an optmization manner
				- You can run the code once with a large range. You can then visually see
				(by Scatter plotting the data), how to narrow your range down, then re-run the code.
				- OR you can just increase the steps to something crazy and basically have an optimizer.
					- I recommend the former approach.
				- Due to how many assumptions this code makes, I find it more useful to 
				see all the data, then narrow the range, and then make a decision on what wing to use,
				rather than just solving for the best wing.
					- For example: If a wing that is 40ft and 70ft perform within .01 seconds 
					of one another, it might make more sense to pick the 70ft wing for structural reasons
					than the technical optimial (but maybe not practical) 40 ft wing.
						- For these reasons the "optimizer" isn't completely automated
						to spit out the best value, but rather a useful set of data points
						that can easily be graphed.

		- Max takeoff AoA for every plane is assumed to be 15 deg
			- Program aborts if plane needs to ever go above this for takeoff

		- Max AoA an airplane will ever have is 15 deg

		- Wing weight is accounted for, but in the optimizer, if a wing weight
		isn't sufficient to have the aircraft have a MTOW of 25,000kg, then
		the difference it needs to get to this benchmark is added to the wing.
			- Program is aborted if MTOW > 35000

		- A lot of functions rely on digitized graphs. This and all the other assumptions are in the README


	********************************************************************

	TLDR of Everything Below this Comment:

		- Read the Github README, then come back to this... to:

		1) See that the code actually compiles and produces what I claim it does
		2) See the implementation of the optimizers
		3) See the implementation of any other function
			- In the implementation file of that particular Class
			- Or in the "Other Functions that might be Intriguing"
				- That section, is "nicer" "debugging" code that can help you see
				what the code is generating for each simulation
					- Like the power curve


			- *** You probably will want to look at Airplane.cpp first ***
				- *** Then look at DragCoeff.cpp ***
					- Basically every class is setup so that it can easily
					  use the DragCoeff functions

				- Also, basically every other class's purpose is to make
				the Airplane.cpp more readable, modular, etc
					- And the Airplane.cpp's purpose is to easily
					write the optimizer

	********************************************************************


	- Below the optimizer function there are other functions that you can uncomment,
	which may help you understand the program better. 


	- Below these other functions, is the optimizers's implementation.


	- All of this code relies HEAVILY on every other class created, as explained in the GitHub.
		- "The classes, which will be described in much more detail in the 
		"Classes and Assumptions" section, whole purpose is to make it easier, 
		among other things (readbility, modularity, etc), to code an optimizer..."
			- Directly quoted from main() function, Overview / Approach portion of README




	- The Header file's don't have traditional (or any useful) header documentation 
		(sorry, it would take forever), so the README is the only place 
		where they are explained

		- Which is also nice, because it is all in one place and 
			formatted nicely, compared to green comments

		- It is also explained in a more "English approach"... whereas
		  header documentation is usually written with the assumption the user
		  is already very familar with the language :)

		- There are comments in implementation files for SOME of the functions...
			- These comments tend to give a general idea of the implementation...
			  instead of explaining it line by line

			- Most comments are reasonings for specific lines (physics reasonings)

			- A lof of these comments are physics derivations that are used in the functions

			- The general consesus is that comments explaining code implementation
			  in a lot of detail, tend to make the code more confusing and less readable
			  than just having the code by itself.
			
			- Again, look at Github, for very brief and Enlgish explanation... then look at the code
			  so that you understand what it is attempting to do
				- The function names do describe what the function does very accurately in most cases.


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



struct wingSweepOptimizerResults {
	// The best wing is stored at index 0
	// wing at wingVector[0] has a timeToClimb of timeToClimbVector[0]
	vector<double> wingSweepVector;     // degrees
	vector<double> climbTimeVector;     // minutes
};




// Function Protypes (What functions are in this file... needed for compiler purposes)
void printUsefulCharacteristics(Wing& inWing, Airplane& inAirplane);
void sortWingsByClimbTime(vector<double>& wingSpans, vector<double>& climbTimes);
void spanOptimizerResultsToCSV(wingSpanOptimizerResults& results, string fileName);
void sweepOptimizerResultsToCSV(wingSweepOptimizerResults& results, string fileName);


wingSpanOptimizerResults spanOptimizer(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage,
	double inFuelWeight, double inPayLoadWeight, double minSpanFt, double maxSpanFt, int numSteps);

wingSpanOptimizerResults spanOptimizerApprox(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage,
	double inFuelWeight, double inPayLoadWeight, double minSpanFt, double maxSpanFt, int numSteps);

wingSweepOptimizerResults sweepOptimizer(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage,
	double inFuelWeight, double inPayLoadWeight, double minSweepDeg, double maxSweepDeg, int numSteps);




// Global Constants:
static constexpr double PI = 3.141592653589;
static constexpr double FEET_TO_INCHES = 12.0;
static constexpr double SECONDS_TO_MINS = 60.0;
static constexpr double MIN_TOTAL_WEIGHT = 25000 * 2.205;     // Need airplane to be at least 25000 kgs
static constexpr double MAX_TOTAL_WEIGHT = 35000 * 2.205;     // Need airplane to be under 35000kgs











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

	double fuselageWeight = 0.95 * givenAirplaneWeight;		   // lbms... we will also store the VT and HT weight in this
	double nacelleWeight = 0.025 * givenAirplaneWeight;		   // lbms.. we will just said each one is 2.5% of fuselage+VT+HT
	double fuselageLength = 51.817 * FEET_TO_INCHES;		   // inches, from CAD
	double fuselageWettedArea = 811.55 * FEET_TO_INCHES;       // inches, from Doc
	double nacelleWettedArea = 159.87 * FEET_TO_INCHES;        // inches, from Doc
	double nacelleLength = 7.27 * FEET_TO_INCHES;              // inches, from CAD
	Nacelle nacelle(nacelleWeight, nacelleLength, nacelleWettedArea);                 
	Fuselage fuselage(fuselageWeight, fuselageLength, fuselageWettedArea); 


	// Main Wing Stuff (only RootChord, TipChord, and Airfoil matter, rest is optmized)
	double mainSpan = 69.35 * FEET_TO_INCHES;				  // inches
	double mainRootChord = 12.0 * FEET_TO_INCHES;			  // inches
	double mainTipChord = 4.8 * FEET_TO_INCHES;				  // inches
	double mainSweepAngle = 20;							  // degrees

	Wing mainWing(NACA2412, mainSpan, mainTipChord, mainRootChord, mainSweepAngle); 
	Airplane airplane(mainWing, HT, VT, CF34_3B1, nacelle, fuselage, startingFuelWeight, payLoadWeight);












/*
	 **********************************************
			Optimizers Used for Report
	(Uncommented, just run program and they will work)
	 **********************************************
*/






/*
	**********************************************
					Span Optimizer 
	   (This one Adjusts velocity while climbing)
	**********************************************


	- If a wing isn't feasible (typically because it is too small), and abort will be called
		- 99% of the time, just increase MIN_SPAN_SIMULATED
			- Abort is usually called bc wing is too small to takeoff
			- Typically any wing span above ~20ft works (depends on your tip and root chords)
				- There are structural concerns for small (and large) wing spans
				that this program does not currently attempt to deal with 
					- Namely because the inertia emperical equations (that I tried implementing)
					  aren't accurate


	- This function uses calcBestTimeTo9km() 
		- Which adjusts V to be at V_maxExcessPower (within an error of course)...
			- What the error is set to can drastically change the climb times...
			- Currently the error is set to a value that gives around the same results as the Approx Climb Method
		- Read Github documentation or go over to Airplane.cpp to understand it's implementation


	- The optimizer essentially runs this simulation for a new aircraft with different traits (defined by the range you give the optimizer)
	- The optimizer also sorts that data, and bla bla bla, but the optimizer code might make more sense, if you understand this snippet of code

*/





	// This is the range the optimizer runs on (Feel free to change)
	wingSpanOptimizerResults spanResults;
	double MIN_SPAN_SIMULATED = 20;     // Min span in ft (Optimizer aborts at below ~20ft)
	double MAX_SPAN_SIMULATED = 150;    // Max span in ft (Optimize aborts at ~461ft+)
	int NUMBER_OF_SIMULATIONS = 25;      // How many evenly spaced steps in the range the optimizer will simulate
									    // 50 Steps ~ 1 min, obv more will take more time... and depends on computer



	cout << "***************************************" << endl;
	cout << "******* Running Span Optimizer: *******" << endl;
	cout << "***************************************" << endl << endl;
	spanResults = spanOptimizer(mainWing, HT, VT, CF34_3B1, nacelle, fuselage, startingFuelWeight, payLoadWeight, 
		MIN_SPAN_SIMULATED, MAX_SPAN_SIMULATED, NUMBER_OF_SIMULATIONS);												// This will print "simulating" while operating


	cout << endl << endl;
	cout << "For the wing with charateristics (ignore the span): " << endl;
	printUsefulCharacteristics(mainWing, airplane);
	cout << "(Note that total weight is adjusted automatically by optimizer (if below 25k kg))" << endl;
	cout << endl << endl;

	cout << "The Span Optimization Results are: " << endl;
	for (int i = 0; i < spanResults.wingSpanVector.size(); i++) {
		cout << fixed << setprecision(5);
		cout << "Time: " << spanResults.climbTimeVector[i] << " mins";
		cout << " Wing Span: " << spanResults.wingSpanVector[i] << " ft" << endl;
		cout << endl;

	}


	spanOptimizerResultsToCSV(spanResults, "Span_OptimizerData.csv");     // Feel Free to change the fileName (2nd parameter)
	cout << endl << endl << endl << endl << endl << endl;






















	/*
		**********************************************
					Sweep Angle Optimizer 
			(This one Adjusts velocity while climbing)
		**********************************************




		- Very similar to spanOptimizer, just with Sweep Angle instead.
			- Typically no aborts will be called, but it is subject to same
			limitations as the span Optimizer


		- This function uses calcBestTimeTo9km()
			- Which adjusts V to be at V_maxExcessPower (within an error of course)...
				- What the error is set to can drastically change the climb times...
				- Currently the error is set to a value that gives around the same results as the Approx Climb Method
			- Read Github documentation or go over to Airplane.cpp to understand it's implementation


		- The optimizer essentially runs this simulation for a new aircraft with different traits (defined by the range you give the optimizer)
		- The optimizer also sorts that data, and bla bla bla, but the optimizer code might make more sense, if you understand this snippet of code

	*/





	// Use this line if you ran the span optimizer, and want to optimize around that span
	//Wing mainWingSweepOptimizer(NACA2412, spanResults.wingSpanVector[0] * FEET_TO_INCHES, mainTipChord, mainRootChord, mainSweepAngle);



	// Otherwise use this line:
	double SPAN_FOR_SWEEP_OPTIMIZER = 69.35 * FEET_TO_INCHES;      // ft, you pick
	Wing mainWingSweepOptimizer(NACA2412, SPAN_FOR_SWEEP_OPTIMIZER, mainTipChord, mainRootChord, mainSweepAngle);




	// This is the range the optimizer runs on
	wingSweepOptimizerResults sweepResults;
	double MIN_SWEEP_ANGLE_SIMULATED = 0;     // deg
	double MAX_SWEEP_ANGLE_SIMULATED = 60;    // deg
	int NUMBER_OF_SIMULATIONS2 = 25;           // How many evenly spaced steps in the range the optimizer will simulate
									            // 50 Simulations ~ 1 min, obv more will take more time... and depends on computer

	cout << "***************************************" << endl;
	cout << "****** Running Sweep Optimizer: *******" << endl;
	cout << "***************************************" << endl << endl;
	sweepResults = sweepOptimizer(mainWingSweepOptimizer, HT, VT, CF34_3B1, nacelle, fuselage, startingFuelWeight, payLoadWeight,
		MIN_SWEEP_ANGLE_SIMULATED, MAX_SWEEP_ANGLE_SIMULATED, NUMBER_OF_SIMULATIONS2);								// This will print "simulating" while operating



	cout << endl << endl;
	cout << "For the wing with charateristics (ignore the sweep): " << endl;
	printUsefulCharacteristics(mainWing, airplane);
	cout << "(Note that total weight is adjusted automatically by optimizer (if below 25k kg))" << endl;
	cout << endl << endl;

	cout << "The Sweep Angle Otimization Results are: " << endl;
	for (int i = 0; i < sweepResults.wingSweepVector.size(); i++) {
		cout << fixed << setprecision(5);
		cout << "Time: " << sweepResults.climbTimeVector[i] << " mins";
		cout << " Wing Sweep: " << sweepResults.wingSweepVector[i] << " deg" << endl;
		cout << endl;

	}

	sweepOptimizerResultsToCSV(sweepResults, "SweepAngle_OptimizerData.csv");  // Feel Free to change the fileName (2nd parameter)
	cout << endl << endl << endl << endl << endl << endl;







/*
	 **********************************************
						End of
				Optimizers Used for Report
	 **********************************************
*/









/*
	**********************************************
					Flight Envelope
	**********************************************


	- Produces a flight envelope for the given airplane... this envelope does not take into account
	structural or other constraints. It just gives the max and min mach that you can fly at
	and the mach for best RoC. 
	- It outputs 2 .csv files. You need to copy over the Flight_Envelope_RoC_Data into the 
	Flight_Envelope_Max_and_Min_Mach_Data, csv. Then you need to plot (Scatter plot)
	the right column as x and the left column as y, for both sets of data. 
	This will produce the envelope.
*/

	airplane.getFlightEnvelopeTo9kmCSV("Flight_Envelope.csv");








/*
	 **********************************************
	   Other Functions that might be Intriguing 
	   (Useful for seeing how the program works)
	 **********************************************
	 
	 - Recommended to Look at: 
		- Power Curve Plot Function
		- 1 Aircraft (the one initialized at the top of the program) Simulation

	- Comment out the optimizer (can just // next to the "results = spanOptimizer..." line)
	- Uncomment the code part of the function you want to look at
		- Just remove the /* and the corresponding * /

	
*/







/*
	 **********************************************
				1 Aircraft Simulation
	(the one initialized at the top of the program)
	 **********************************************

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
	**********************************************
		   Power Curve Plot Function 
	**********************************************

	- It will produce .csv data for the Airplane intialized above at the given height
	- The .csv will be saved in the to-9km-and-beyond folder
	- Just open the .csv and select the A, B and C columns and create a scatter plot, in order to
		the Power Curve (Drag Bucket) for the given airplane at the provided height
	- Make sure you close the .csv or rename the fileNamePowerCurve before re-running
	- Power Curves like this one are used to find the maxExcessPower, currentExcessPower, etc
		- See Github documentation for a more detailed explanation... this occurs in "Airplane.cpp"
*/

/*
	double heightPowerCurveFunction = 5000;
	string fileNamePowerCurve = "PowerCurve.csv";
	airplane.getPowerCurveCSV(heightPowerCurveFunction, fileNamePowerCurve);
*/















/*

	**********************************************
		   Sanity Check Span Optimizer
	(Does not adjust Velocity while climbing)
	**********************************************


	- This is used as a sanity check to make sure the adjusted velocity function
		isn't massively different than this simplistic approach

	- Works essentially the same as the other Span Optimizer, but calls an optimizer that uses
	calcRoughApproxTimeTo9km() instead of calcBestTimeTo9km()



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
	 **********************************************
					End of 
	   Other Functions that might be Intriguing
	 **********************************************
*/






	return 0;
}



















/*
		**********************************************
		***** Start of Optimizers used for Report ****
		******** (Implementation of them) ************
		**********************************************
*/









// Uses calcBestTimeTo9km (the approach that adjusts velocity to = velocity for maxExcessPower)
wingSpanOptimizerResults spanOptimizer(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage, 
										double inFuelWeight, double inPayLoadWeight, double minSpanFt, double maxSpanFt, int numSteps) {

	// Inputted Constants:
	double SPAN_MIN = minSpanFt * FEET_TO_INCHES;			// No more breaking... unless abort
	double SPAN_MAX = maxSpanFt * FEET_TO_INCHES;			// Run it high, then adjust (*12 to convert to inches)
	int NUM_STEPS = numSteps;				    // NUM_STEPS = 100 takes about 1 min and 15s to run

	// Other Constants:
	double TAKE_OFF_END_HEIGHT = 500;   // Change to 0, 10, or 50ft when wanting to get best possible plane
	double START_HEIGHT = 0;            // Don't change
	double START_VELOCITY = 0;          // Don't Change

	// Other Intialization
	Wing newWing(inWing);						 // Calls copy constructor (equivalent to newWing = inWing)
	wingSpanOptimizerResults returnStruct;
	vector<double> wingSpansVector;
	vector<double> climbTimeVector;


	double step = (SPAN_MAX - SPAN_MIN) / NUM_STEPS;


	for (int i = 0; i < NUM_STEPS; i++) {
		double newSpan = SPAN_MIN + (i * step);
		newWing.setSpan(newSpan);

		cout << "Simulating Span (ft): " << newSpan / FEET_TO_INCHES << endl; // can delete, but useful for making sure you aren't stuck in an infinite loop

		Airplane newAirplane(newWing, inHT, inVT, inEngine, inNacelle, inFuselage, inFuelWeight, inPayLoadWeight);

		double totalWeight = newAirplane.getWeight();
		if (totalWeight < MIN_TOTAL_WEIGHT) {
			double weightNeeded = MIN_TOTAL_WEIGHT - totalWeight;
			newAirplane.setMainWingWeight(newAirplane.getMainWingWeight() + weightNeeded);
		}

		assert(newAirplane.getWeight() >= MIN_TOTAL_WEIGHT - .1 && newAirplane.getWeight() <= MAX_TOTAL_WEIGHT);

		double climbTime = newAirplane.calcBestTimeTo9km(START_HEIGHT, TAKE_OFF_END_HEIGHT);


		/* 
			// Unforunately the emperical equations used are not accurate enough
			// But this would check if our Plane can handle the stress required
			if (!newAirplane.isWingPossible()) {
			climbTime = 9e9; // Very large climb time
			}
		*/


		wingSpansVector.push_back(newSpan / FEET_TO_INCHES);     // stored in ft
		//climbTimeVector.push_back(climbTime);  // stored in seconds
		climbTimeVector.push_back(climbTime / SECONDS_TO_MINS);  // stored in mins
	}




	sortWingsByClimbTime(wingSpansVector, climbTimeVector);

	returnStruct.wingSpanVector = wingSpansVector;
	returnStruct.climbTimeVector = climbTimeVector;
	return returnStruct;
}

















// Runs Simulations for airplanes from minSweepAngle to maxSweepAngle and returns the sorted results.
// Will abort if one of the wings isn't possible (not the most ideal handling, but just change inputted bounds to fix)
wingSweepOptimizerResults sweepOptimizer(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage,
	double inFuelWeight, double inPayLoadWeight, double minSweepDeg, double maxSweepDeg, int numSteps) {

	// Constants:
	double TAKE_OFF_END_HEIGHT = 500.0;					// Change to 0, 10, or 50ft when wanting to get best possible plane
	double START_HEIGHT = 0.0;							// Don't change
	double START_VELOCITY = 0.0;							// Don't Change


	// Other Intialization
	Wing newWing(inWing);						 // Calls copy constructor (equivalent to newWing = inWing)
	wingSweepOptimizerResults returnStruct;
	vector<double> wingSweepVector;
	vector<double> climbTimeVector;


	double step = (maxSweepDeg - minSweepDeg) / numSteps;


	for (int i = 0; i < numSteps; i++) {
		double newSweep = minSweepDeg + (i * step);

		newWing.setSweepAngle(newSweep);						// takes in degrees

		cout << "Simulating Sweep (deg): " << newSweep << endl; // can delete, but useful for making sure you aren't stuck in an infinite loop

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


		wingSweepVector.push_back(newSweep);     // stored in deg
		//climbTimeVector.push_back(climbTime);  // stored in seconds
		climbTimeVector.push_back(climbTime / SECONDS_TO_MINS);  // stored in mins
	}





	sortWingsByClimbTime(wingSweepVector, climbTimeVector);

	returnStruct.wingSweepVector = wingSweepVector;
	returnStruct.climbTimeVector = climbTimeVector;
	return returnStruct;

}







/*
		**********************************************
		***** End of Optimizers used for Report ******
		**********************************************
*/

























/*
		**********************************************
		** Start of Other Used Functions in Program **
		**********************************************
*/





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

























// Creates a CSV file with the span Optimizer Results
void spanOptimizerResultsToCSV(wingSpanOptimizerResults& results, string fileName) {
	vector<double> attemptNumber;
	for (int i = 0; i < results.wingSpanVector.size(); i++) {
		attemptNumber.push_back(i + 1);
	}
	saveVectorsToCSV(attemptNumber, results.wingSpanVector, results.climbTimeVector, fileName);

	return;
}

















// Creates a CSV file with the Sweep Optimizer results
void sweepOptimizerResultsToCSV(wingSweepOptimizerResults& results, string fileName) {
	vector<double> attemptNumber;
	for (int i = 0; i < results.wingSweepVector.size(); i++) {
		attemptNumber.push_back(i + 1);
	}
	saveVectorsToCSV(attemptNumber, results.wingSweepVector, results.climbTimeVector, fileName);

	return;
}













// Useful for debugging
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
		**********************************************
		*** End of Other Used Functions in Program ***
		**********************************************
*/
























/*
		**********************************************
		********* Start of Approx Optimizers *********
		**********************************************
*/





// Uses calcRoughApproxTimeTo9km (the approach that get's to original maxExcessPower velocity, and stays at it)
wingSpanOptimizerResults spanOptimizerApprox(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage,
	double inFuelWeight, double inPayLoadWeight, double minSpanFt, double maxSpanFt, int numSteps) {

	// Adjustable Constants:
	double SPAN_MIN = minSpanFt * FEET_TO_INCHES;			// No more breaking... unless abort
	double SPAN_MAX = maxSpanFt * FEET_TO_INCHES;			// Run it high, then adjust (*12 to convert to inches)
	int NUM_STEPS = numSteps;				// NUM_STEPS = 100 takes about 1 min and 15s to run

	double TAKE_OFF_END_HEIGHT = 500;   // Change to 0, 10, or 50ft when wanting to get best possible plane
	double START_HEIGHT = 0;            // Don't change
	double START_VELOCITY = 0;          // Don't Change

	// Other Intialization
	Wing newWing(inWing);						 // Calls copy constructor (equivalent to newWing = inWing)
	wingSpanOptimizerResults returnStruct;
	vector<double> wingSpansVector;
	vector<double> climbTimeVector;


	double step = (SPAN_MAX - SPAN_MIN) / NUM_STEPS;

	for (int i = 0; i < NUM_STEPS; i++) {
		double newSpan = SPAN_MIN + (i * step);
		newWing.setSpan(newSpan);

		cout << "Simulating Span (ft): " << newSpan / FEET_TO_INCHES << endl; // delete

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

		wingSpansVector.push_back(newSpan / FEET_TO_INCHES);     // stored in ft
		//climbTimeVector.push_back(climbTime);  // stored in seconds
		climbTimeVector.push_back(climbTime / SECONDS_TO_MINS);  // stored in mins
	}




	sortWingsByClimbTime(wingSpansVector, climbTimeVector);

	returnStruct.wingSpanVector = wingSpansVector;
	returnStruct.climbTimeVector = climbTimeVector;
	return returnStruct;
}





/*
		**********************************************
		********* End of Approx Optimizers *********
		**********************************************
*/































// Old implementaitons (ignore):



/*


	// Less passed parameters (Span_min, span_max, and numSteps set)
	wingSpanOptimizerResults spanOptimizer(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage,
		double inFuelWeight, double inPayLoadWeight) {

		// Adjustable Constants:
		double SPAN_MIN = 8 * FEET_TO_INCHES;			// No more breaking... unless abort
		double SPAN_MAX = 100 * FEET_TO_INCHES;			// Run it high, then adjust (*12 to convert to inches)
		int NUM_STEPS = 25;				// NUM_STEPS = 100 takes about 1 min and 15s to run

		double TAKE_OFF_END_HEIGHT = 500;   // Change to 0, 10, or 50ft when wanting to get best possible plane
		double START_HEIGHT = 0;            // Don't change
		double START_VELOCITY = 0;          // Don't Change

		// Other Intialization
		Wing newWing(inWing);						 // Calls copy constructor (equivalent to newWing = inWing)
		wingSpanOptimizerResults returnStruct;
		vector<double> wingSpansVector;
		vector<double> climbTimeVector;

		double step = (SPAN_MAX - SPAN_MIN) / NUM_STEPS;

		for (int i = 0; i < NUM_STEPS; i++) {
			double newSpan = SPAN_MIN + (i * step);
			newWing.setSpan(newSpan);

			cout << "Simulating Span (ft): " << newSpan / FEET_TO_INCHES << endl; // delete

			Airplane newAirplane(newWing, inHT, inVT, inEngine, inNacelle, inFuselage, inFuelWeight, inPayLoadWeight);

			double totalWeight = newAirplane.getWeight();
			if (totalWeight < MIN_TOTAL_WEIGHT) {
				double weightNeeded = MIN_TOTAL_WEIGHT - totalWeight;
				newAirplane.setMainWingWeight(newAirplane.getMainWingWeight() + weightNeeded);
			}

			assert(newAirplane.getWeight() >= MIN_TOTAL_WEIGHT - .1);

			double climbTime = newAirplane.calcBestTimeTo9km(START_HEIGHT, TAKE_OFF_END_HEIGHT);
			wingSpansVector.push_back(newSpan / FEET_TO_INCHES);     // stored in ft
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