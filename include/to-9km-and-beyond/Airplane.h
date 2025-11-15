#ifndef AIRPLANE_H
#define AIRPLANE_H
#include <string>
using std::string;
#include <vector>
using std::vector;
#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/CF34_3B1.h"
#include "to-9km-and-beyond/Nacelle.h"
#include "to-9km-and-beyond/Fuselage.h"


// Assumes that plane is symmetric and has 2 engines (wings are symmetrical)
// Assumes VT makes negligible lift and drag
// Can re-write some functions to take in height instead so I can use AtmosphereProp Class behind the scences

namespace airplane {
	class Airplane {
	public:
		Airplane();
		Airplane(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage, double inFuelWeight, double inPayLoadWeight);

		double calcMach(double velocity, double temp) const;

		double calcDragCoeff(double AoA, double velocity, double Mach, double kinematicViscosity) const;				// AoA in Rad
		double calcDrag(double AoA, double velocity, double Mach, double kinematicViscosity, double density) const;     // AoA in Rad

		double calcLiftCoeff(double AoA) const;																			// AoA in Rad
		double calcLift(double AoA, double velocity, double density) const;												// AoA in Rad

		// Climb Functions
		double calcBestClimbTime(double startHeight, double endHeight) const;               // "Best" means we somehow get to the V we need to get to instaneously
		double calcSteadyClimbAoA(double gamma, double velocity, double density) const;     // Gamma in degrees, Returns AoA in rad


		// Power Curve
		void getPowerCurveCSV(double gamma, double height, string fileName) const;           // fileName should have ".csv"


		// Accessors:
		double getWeight() const;



	private:
		double totalWeight;      // Class calculates this
		double referenceArea;    // Class assigns this the area of the main wing
		Wing* HT;                // Full HT (Full Span / Planform)
		Wing* VT;                // Full VT (Full Span / Planform)
		Wing* mainWing;          // Full mainWing (Full Span / Planform)
		CF34_3B1* engine;        // One engine (Class assumes plane has 2 identical engines)
		Nacelle* nacelle;        // One nacelle (Class assumes plane has 2 identical nacelles)
		Fuselage* fuselage;      // One fuselage
		double payLoadWeight;    //
		double fuelWeight;       // Weight of Fuel
		LiftCoeff CL;           // One CL for the whole plane

		const static int numEngines = 2;

		static constexpr double GAS_CONSTANT = 1716;
		static constexpr double pi = 3.141592653589;

		// Power Function's Curve Constants
		static constexpr double xmin = .005;
		static constexpr double xmax = .975;
		const static int steps = 1000;



		// computeAndSet Functions:
		void calcAndSetTotalWeight();
		void calcAndSetLiftCoeff();													 // Needed for SteadyClimbAoA... sets Cl


		// Climb Functions
		double calcMaxExcessPower(double startHeight, double endHeight) const;   // Prob also need max excess power speed
		

		// Power Curve Functions
		vector<double> calcDragPowerCurveYData(double gamma, double height) const;           // Gamma in degrees, generates 1000 evenly spaced data points between Mach 0 and 1
		vector<double> calcDragPowerCurveXDataMach() const;									 // Will Always be the same
		vector<double> calcDragPowerCurveXDataVel(double height) const;						 // Power Curve needs velocity
	};




}


#endif