#include <iostream>
#include <string>
#include "to-9km-and-beyond/Airplane.h"
#include "to-9km-and-beyond/AtmosphereProperties.h"
#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/Airfoil.h"
#include "to-9km-and-beyond/Nacelle.h"
#include "to-9km-and-beyond/Fuselage.h"
#include "to-9km-and-beyond/CF34_3B1.h"



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


	-In the Airplane Class
		- Need to write a getWeight function that takes in a time and gets my weight, which will use the enginge stuff
		- Probably need a gamma calc to get Max RoC


	- Need to write a takeoff time thing
	- Need to write ROC thing


	- General Idea for optimizing
		1. Optimize span
		2. Optimize airfoil (need a alpha zero AoA for NACA to do this)

*/




// Useful main
int main() {
	Airfoil NACA2412("2412", -.0349);

	// Other Variables
	double currentHeight = 0;
	double time = 0;


	// Givens:
	Wing VT(NACA2412, 99.68, 86.68, 86.68, 40, 0);										// Set in Stone.. Altho not really needed bc ignoring contributions
	Wing HT(NACA2412, 239.67, 23.55, 72.45, 32.04, 0);									// Set in Stone
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
	double mainSpan = 1176;
	double mainRootChord = 186.7;
	double mainTipChord = 74.7;
	double mainSweepAngle = 20;
	Wing mainWing(NACA2412, mainSpan, mainTipChord, mainRootChord, mainSweepAngle);

	// Airplane we have with everything
	Airplane Airplane(mainWing, HT, VT, CF34_3B1, nacelle, fuselage, startingFuelWeight, payLoadWeight);




	// Testing CL3D Wing
	AtmosphereProperties Cond(0);
	double temp = Cond.getTemperature();
	double kineVisc = Cond.getKinematicVisc();
	double density = Cond.getDensity();



	double gamma = 7.5; // Deg
	double velocity = 1004.669673; // ft/s

	double Mach = Airplane.calcMach(velocity, temp);
	double AoA = Airplane.calcSteadyClimbAoA(gamma, velocity, density);



	/*
	cout << "Main Wing: " << endl;
	cout << "Area: " << mainWing.getArea() << endl;
	cout << "AR: " << mainWing.getAspectRatio() << endl;
	cout << "e: " << mainWing.getEllipticalEffic() << endl;
	cout << "Sweep: " << mainSweepAngle << endl;
	cout << "MAC: " << mainWing.getMAC() << endl;
	cout << "CL3D Wing total: " << mainWing.calcLiftCoeff(AoA) << endl;


	cout << endl << endl << endl;
	cout << "HT: " << endl;
	cout << "Area: " << HT.getArea() << endl;
	cout << "AR: " << HT.getAspectRatio() << endl;
	cout << "e: " << HT.getEllipticalEffic() << endl;
	cout << "Sweep: " << HT.getSweepAngle() << endl;
	cout << "CL3D HT total: " << HT.calcLiftCoeff(AoA) << endl;

	cout << endl << endl << endl;
	cout << "Fuselage Lift: " << fuselage.calcLiftCoeff(AoA) << endl;

	cout << endl << endl;
	*/


	cout << "Gamma: " << gamma << endl;
	cout << "Mach: " << Mach << endl;
	cout << "AoA Required " << AoA * 180 / 3.1415 << endl;
	cout << "Airplane Drag Coeff: " << Airplane.calcDragCoeff(AoA, velocity, Mach, kineVisc) << endl;
	cout << "Airplane Drag: " << Airplane.calcDrag(AoA, velocity, Mach, kineVisc, density) << endl;





	return 0;
}











