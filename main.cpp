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
	givenAirplaneWeight = givenAirplaneWeight - startingFuelWeight - payLoadWeight - (2*1670);     // 1670 lbms is CF34_3B1
	double fuselageWeight = .95 * givenAirplaneWeight;									// lbms... we will also store the VT and HT weight in this
	double nacelleWeight = .025 * givenAirplaneWeight;									// just said each one is 2.5% of fuselage+VT+HT
	double fuselageLength = 216;														// inches
	double fuselageWettedArea = 811.55 * 12;                                            // inches
	Nacelle nacelle(nacelleWeight);
	Fuselage fuselage(fuselageWeight, fuselageLength, fuselageWettedArea);
	CF34_3B1 CF34_3B1;

	// Main Wing Stuff, this is what will be optimized
	double mainSpan = 1176;
	double mainRootChord = 186.7;
	double mainTipChord = 74.7;
	double mainSweepAngle = 20;
	Wing mainWing(NACA2412, mainSpan, mainTipChord, mainRootChord, mainSweepAngle);      // mainWing weight will be calculated by airplane

	// Airplane we have with everything
	Airplane airplane(mainWing, HT, VT, CF34_3B1, nacelle, fuselage, startingFuelWeight, payLoadWeight);

	double startHeight = 0;
	double takeOffEndHeight = 500;





// Optimizer Stuff
	/*
		Approach:
		0) Fix Airfoil
		1) Fix ct and cr to reasonable values and optimize span
		2) Fix span at the top 3 best values and optimize ct and cr (use a nester loop for this)
		3) Fix all the previous variables and optimize sweep angle
		4) Re-run from start with different airfoisl
		
		Notes:
			- Need to make sure min and max weight are adhered to and airplanes not adhereing are scrapped
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




/*
vector<Airplane*> spanOptimizer(Airplane& airplane) {
	double SPAN_MIN = 12 * 12;      // just saying 144 inches rn (in reality higher)
	double SPAN_MAX = 350 * 12;     // ~265 is the max.. but will let optimizer try up to 350
	vector<Airplane*> spanOptimizedResults;


	int NUM_STEPS = 100;
	double step = (SPAN_MAX - SPAN_MIN) / NUM_STEPS;

	for (int i = 0; i < NUM_STEPS; i += step) {

	}

	return spanOptimizedResults;
}
*/

















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