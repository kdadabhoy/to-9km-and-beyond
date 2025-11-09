#include <iostream>

#include "to-9km-and-beyond/Nacelle.h"
#include "to-9km-and-beyond/Airplane.h"
#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/TurboFan.h"
#include "to-9km-and-beyond/Fuselage.h"
#include "to-9km-and-beyond/AtmosphereProperties.h"


using namespace std;
using namespace atmosphere_properties;
using namespace airplane;
using namespace turbofan;

/*
	To Do List:

	- Need to Create Aero Coefficients
	- Probably need to create an Airfoil Class 


	- In Wing Class
		- Need to incoporate the Airfoil class into my Wings
			- Need to choose an airfoil and run with it for now
		- Need to add a public reynolds number function
		- Need to add a total drag fucntion (given a velocity and density)
		- Wetted Area helper function



	- In TurboFan class:
		- Need to digitalize my plots
		- Need to write the fuel consumption / Thrust and mach number curve stuff into my engine class
		- Need to have a Power vs Speed curve for all atmospheres
			- And need to be able to tell me the speed that will have the max excess power

	-In the Airplane Class
		- Need to get my total Lift and total Drag given a velocity by calling on Wing Drag/Lift functions
		- Need to write a getWeight function that takes in a time and gets my weight, which will use the enginge stuff
		- Probably need a gamma calc to get Max RoC


	- Need to write a takeoff time thing
	- Need to write ROC thing


	- General Idea for optimizing
		1. Optimize span
		2. Optimize airfoil

*/




/* 
	Useful to know :
	Airplane(inWing, inHT, inVT, inEngine, inNacelle, inFuselage, inFuelWeight, inPayLoadWeight);
	Wing(inSpan, inTipChord, inRootChord, inSweepAngle, weight), weight is optional.. if you dont add it then it will auto calc it for you
	

*/





// Useful main
int main() {
	// Airfoil characteristics for my HT and VT:
	// Let's use a NACA 2412 for starters
	
	// Givens:
	Wing VT(99.68, 86.68, 86.68, 40, 0);												// Set in Stone
	Wing HT(239.67, 23.55, 72.45, 32.04, 0);											// Set in Stone
	double givenAirplaneWeight = 23000 * 2.20462;										// lbms, this it the HT + VT + Fuselage + startingFuel + Payload
	double startingFuelWeight = 1000 * 2.20462;											// lbm
	double payLoadWeight = 1000 * 2.20462;												// lbm
	givenAirplaneWeight = givenAirplaneWeight - startingFuelWeight - payLoadWeight;
	double fuselageWeight = .9 * givenAirplaneWeight;									// lbms... we will also store the VT and HT weight in this
	double nacelleWeight = .05 * givenAirplaneWeight / 2;                               // just said each one is 2.5% of fuselage+VT+HT
	Fuselage fuselage(fuselageWeight);
	Nacelle nacelle(nacelleWeight);


	double currentHeight = 0;
	double time = 0;


	// Main Wing Stuff, this is what will be optimized
	double mainSpan = 100;
	double mainTipChord = 100;
	double mainRootChord = 100;
	double mainSweepAngle = 30;
	Wing Wing(mainSpan, mainTipChord, mainRootChord, mainSweepAngle);


	//TurboFan CF34_3B1();








	return 0;
}













/*

// Testing AtmosphereProperties 

int main() {
	AtmosphereProperties Cond(49000);
	cout << "Height: " << Cond.getHeight() << " Density: " << Cond.getDensity() << " Pressure : " <<
		Cond.getStaticPressure() << " Temp: " << Cond.getTemperature() <<
		" Speed of Sound: " << Cond.getSpeedOfSound() << endl;

	return 0;
}

*/






/*
	Testing Wing Class:
	double MAC;
	double weight;
	double AR;
	double TR;
	double area;

	MAC = HT.getMAC() * 12;
	weight = HT.getWeight();
	AR = HT.getAspectRatio();
	TR = HT.getTaperRatio();
	area = HT.getArea();

	cout << "MAC: " << MAC << " in" << endl;
	cout << "AR: " << AR << endl;
	cout << "Taper Ratio: " << TR << endl;

	cout << "Area: " << area << " sqft" << endl;
	cout << "Weight: " << weight << " lbm" << endl;
	




*/