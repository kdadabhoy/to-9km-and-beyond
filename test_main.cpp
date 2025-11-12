// Test main


#include <iostream>
#include <string>

#include "to-9km-and-beyond/Airplane.h"
#include "to-9km-and-beyond/AtmosphereProperties.h"
#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/Airfoil.h"
#include "to-9km-and-beyond/Nacelle.h"
#include "to-9km-and-beyond/Fuselage.h"
#include "to-9km-and-beyond/CF34_3B1.h"
#include "to-9km-and-beyond/LiftCoeff.h"



using namespace std;
using namespace atmosphere_properties;
using namespace airplane;



/*
	To Do List:

	- In Wing Class
		- Need to add a C_DC function
			- This requires a Mach Critical Crest #
			- This requires digitizing plots 	



	- In TurboFan class:
		- Need to digitalize my plots
		- Need to write the fuel consumption / Thrust and mach number curve stuff into my engine class
		- Need to have a Power vs Speed curve for all atmospheres
			- And need to be able to tell me the speed that will have the max excess power
			- Prob a main() function

	- In Fuselage class:
		- Need to calculate parasitic drag from fuselage
		- Need to calculate lift from fuselage


	-In the Airplane Class
		- Need to get my total Lift and total Drag given a velocity by calling on Wing Drag/Lift functions
		- Need to write a getWeight function that takes in a time and gets my weight, which will use the enginge stuff
		- Probably need a gamma calc to get Max RoC


	- Need to write a takeoff time thing
	- Need to write ROC thing


	- General Idea for optimizing
		1. Optimize span
		2. Optimize airfoil (need a alpha zero AoA for NACA to do this)

*/




/* 
	Useful to know :
	Airplane(inWing, inHT, inVT, inEngine, inNacelle, inFuselage, inFuelWeight, inPayLoadWeight);
	Wing(Airfoil, inSpan, inTipChord, inRootChord, inSweepAngle, weight), everything in inches, weight is optional.. if you dont add it then it will auto calc it for you
	Airfoil(string inNACA, double inAlphaZeroLift);


*/



/*

// Useful main
int main() {
	// Airfoil characteristics for my HT and VT:
	// Let's use a NACA 2412 for starters.. 
		// bc need to implement 2D zero alpha
	//LiftCoeff Cl_NACA2412(.178, 6.25);
	Airfoil NACA2412("2412", -.0349);



	// Other Variables
	double currentHeight = 0;
	double time = 0;


	// Givens:
	Wing VT(NACA2412, 99.68, 86.68, 86.68, 40, 0);												// Set in Stone
	Wing HT(NACA2412, 239.67, 23.55, 72.45, 32.04, 0);											// Set in Stone
	double givenAirplaneWeight = 23000 * 2.20462;										// lbms, this it the HT + VT + Fuselage + startingFuel + Payload
	double startingFuelWeight = 1000 * 2.20462;											// lbm
	double payLoadWeight = 1000 * 2.20462;												// lbm
	givenAirplaneWeight = givenAirplaneWeight - startingFuelWeight - payLoadWeight - (2*1670);     // 1670 lbms is CF34_3B1
	double fuselageWeight = .95 * givenAirplaneWeight;									// lbms... we will also store the VT and HT weight in this
	double nacelleWeight = .025 * givenAirplaneWeight;                               // just said each one is 2.5% of fuselage+VT+HT
	Fuselage fuselage(fuselageWeight);
	Nacelle nacelle(nacelleWeight);
	CF34_3B1 CF34_3B1;

	// Main Wing Stuff, this is what will be optimized
	double mainSpan = 120;
	double mainTipChord = 12;
	double mainRootChord = 24;
	double mainSweepAngle = 15;
	Wing mainWing(NACA2412, mainSpan, mainTipChord, mainRootChord, mainSweepAngle);

	// Airplane we have with everything
	Airplane Airplane(mainWing, HT, VT, CF34_3B1, nacelle, fuselage, startingFuelWeight, payLoadWeight);

	// Testing CL3D Wing
	double AoA = 5 * 3.1415 / 180;
	AtmosphereProperties Cond(0);
	double temp = Cond.getTemperature();
	double kineVisc = Cond.getKinematicVisc();
	double velocity = 500;
	double Mach = Airplane.calcMach(velocity, temp);

	cout << "Area: " << mainWing.getArea() << endl;
	cout << "AR: " << mainWing.getAspectRatio() << endl;
	cout << "e: " << mainWing.getEllipticalEffic() << endl;
	cout << "Sweep: " << mainSweepAngle << endl;
	cout << "MAC: " << mainWing.getMAC() << endl;
	cout << "AoA rad: " << AoA << endl;
	cout << "CL3D total: " << mainWing.getC_L_rad(AoA) << endl;


	double Re = mainWing.calcReynolds(velocity, kineVisc);
	double wetAreaRatio = mainWing.calcWettedArea() / mainWing.getArea();
	double totalDragCoeff = mainWing.getTotalC_D_rad(AoA, velocity, Mach, wetAreaRatio, kineVisc);

	cout << endl << endl << endl;
	cout << "kineVisc: " << kineVisc << endl;
	cout << "velocity: " << velocity << endl;
	cout << "temp: " << temp << endl << endl;

	cout << "Re: " << Re << endl;
	cout << "C_D: " << totalDragCoeff << endl;


	return 0;
}



*/




















/*
	// Testing CL3D Wing & Getting Drag from Wing member function

	// Main Wing Stuff, this is what will be optimized
	double mainSpan = 120;
	double mainTipChord = 12;
	double mainRootChord = 24;
	double mainSweepAngle = 15;
	Wing mainWing(NACA2412, mainSpan, mainTipChord, mainRootChord, mainSweepAngle);

	// Airplane we have with everything
	Airplane Airplane(mainWing, HT, VT, CF34_3B1, nacelle, fuselage, startingFuelWeight, payLoadWeight);

	// Testing CL3D Wing
	double AoA = 5 * 3.1415 / 180;
	AtmosphereProperties Cond(0);
	double temp = Cond.getTemperature();
	double kineVisc = Cond.getKinematicVisc();
	double velocity = 500;
	double Mach = Airplane.calcMach(velocity, temp);

	cout << "Area: " << mainWing.getArea() << endl;
	cout << "AR: " << mainWing.getAspectRatio() << endl;
	cout << "e: " << mainWing.getEllipticalEffic() << endl;
	cout << "Sweep: " << mainSweepAngle << endl;
	cout << "MAC: " << mainWing.getMAC() << endl;
	cout << "AoA rad: " << AoA << endl;
	cout << "CL3D total: " << mainWing.getC_L_rad(AoA) << endl;


	double Re = mainWing.calcReynolds(velocity, kineVisc);
	double wetAreaRatio = mainWing.calcWettedArea() / mainWing.getArea();
	double totalDragCoeff = mainWing.getTotalC_D_rad(AoA, velocity, Mach, wetAreaRatio, kineVisc);

	cout << endl << endl << endl;
	cout << "kineVisc: " << kineVisc << endl;
	cout << "velocity: " << velocity << endl;
	cout << "temp: " << temp << endl << endl;

	cout << "Re: " << Re << endl;
	cout << "C_D: " << totalDragCoeff << endl;


*/





/*
	// Testing Weight Assignments

void weightCheck(Wing& VT, Wing HT, double fuel, double payload, Fuselage fuselage, Nacelle nacelle, CF34_3B1 engine);
int main() {
	// Airfoil characteristics for my HT and VT:
	// Let's use a NACA 2412 for starters

	double currentHeight = 0;
	double time = 0;


	// Givens:
	Wing VT(99.68, 86.68, 86.68, 40, 0);												// Set in Stone
	Wing HT(239.67, 23.55, 72.45, 32.04, 0);											// Set in Stone
	double givenAirplaneWeight = 23000 * 2.20462;										// lbms, this it the HT + VT + Fuselage + startingFuel + Payload
	double startingFuelWeight = 1000 * 2.20462;											// lbm
	double payLoadWeight = 1000 * 2.20462;												// lbm
	givenAirplaneWeight = givenAirplaneWeight - startingFuelWeight - payLoadWeight - (2 * 1670);     // 1670 lbms is CF34_3B1
	double fuselageWeight = .95 * givenAirplaneWeight;									// lbms... we will also store the VT and HT weight in this
	double nacelleWeight = .025 * givenAirplaneWeight;                               // just said each one is 2.5% of fuselage+VT+HT
	Fuselage fuselage(fuselageWeight);
	Nacelle nacelle(nacelleWeight);
	CF34_3B1 CF34_3B1;

	// Main Wing Stuff, this is what will be optimized
	double mainSpan = 100;
	double mainTipChord = 100;
	double mainRootChord = 100;
	double mainSweepAngle = 30;
	Wing mainWing(mainSpan, mainTipChord, mainRootChord, mainSweepAngle);

	// Airplane we have with everything
	Airplane MyAirplane(mainWing, HT, VT, CF34_3B1, nacelle, fuselage, startingFuelWeight, payLoadWeight);


	weightCheck(VT, HT, startingFuelWeight, payLoadWeight, fuselage, nacelle, CF34_3B1);
	double check = MyAirplane.getWeight();
	cout << check << " " << (23000 * 2.20462) << endl;



	return 0;
}


void weightCheck(Wing& VT, Wing HT, double fuel, double payload, Fuselage fuselage, Nacelle nacelle, CF34_3B1 engine) {
	double givenWeight = 23000 * 2.20462;
	double weight = VT.getWeight() + (2 * HT.getWeight()) + fuel + payload + fuselage.getWeight() + (2 * nacelle.getWeight()) + (2 * engine.getWeight());


	if ((weight - givenWeight) < .001) {
		cout << "Weight calcs are right" << endl;
		return;
	} else {
		cout << "Weight calcs are wrong" << endl;
		return;
	}

}

	*/






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