#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
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

double calcTimeTo9km(Airplane& Airplane, double startHeight, double takeOffEndHeight);


/*
	To Do List:

	- In TurboFan class:
		- Need to digitalize my plots
		- Need to write the fuel consumption / Thrust and mach number curve stuff into my engine class


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
	double nacelleWeight = .025 * givenAirplaneWeight;									// just said each one is 2.5% of fuselage+VT+HT
	double fuselageLength = 216;														// inches
	double fuselageWettedArea = 811.55 * 12;                                            // inches
	Nacelle nacelle(nacelleWeight);
	Fuselage fuselage(fuselageWeight, fuselageLength, fuselageWettedArea);
	CF34_3B1 CF34_3B1;

	// Main Wing Stuff, this is what will be optimized
	double mainSpan = 200;
	double mainRootChord = 186.7;
	double mainTipChord = 74.7;
	double mainSweepAngle = 20;
	Wing mainWing(NACA2412, mainSpan, mainTipChord, mainRootChord, mainSweepAngle);      // mainWing weight will be calculated by airplane

	// Airplane we have with everything
	Airplane Airplane(mainWing, HT, VT, CF34_3B1, nacelle, fuselage, startingFuelWeight, payLoadWeight);

	double startHeight = 0;
	double takeOffEndHeight = 500;

	cout << fixed << setprecision(5);
	cout << "Area: " << mainWing.getArea() << endl;
	cout << "AR: " << mainWing.getAspectRatio() << endl;
	cout << "e: " << mainWing.getEllipticalEffic() << endl;
	cout << "Sweep: " << mainSweepAngle << endl;
	cout << "MAC: " << mainWing.getMAC() << endl;
	cout << "Taper Ratio: " << mainWing.getTaperRatio() << endl;
	cout << "Wing Weight: " << mainWing.getWeight() << endl;
	cout << "Total Weight lbm: " << Airplane.getWeight() << endl;
	cout << "Total Weight in kg: " << Airplane.getWeight() / 2.20462 << endl;
	cout << calcTimeTo9km(Airplane, startHeight, takeOffEndHeight) << " mins" << endl;
	//Airplane.getPowerCurveCSV(0, "exp.csv");

	return 0;
}







// Calcs the time to 9km (accurately) from h=0 to h = 9km + startHeight (accounts for possibility of not starting at sea level)
double calcTimeTo9km(Airplane& Airplane, double startHeight, double takeOffEndHeight) {
	double endHeight = 29527.6 + startHeight; // 9km in ft
	double totalTime = 0;
	double velocity = 0;

	Airplane.calcTakeoffPropertites(startHeight, takeOffEndHeight, totalTime, velocity);
	totalTime += Airplane.calcBestClimbTime(takeOffEndHeight, velocity, endHeight);

	return totalTime / 60;
}






