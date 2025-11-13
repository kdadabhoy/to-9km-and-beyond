#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/TurboFan.h"
#include "to-9km-and-beyond/Nacelle.h"
#include "to-9km-and-beyond/Fuselage.h"


// Assumes that plane is symmetric and has 2 engines (wings are symmetrical)
// Assumes VT makes negligible lift and drag
// Can re-write some functions to take in height instead so I can use AtmosphereProp Class behind the scences

namespace airplane {
	class Airplane {
	public:
		Airplane();
		Airplane(Wing& inWing, Wing& inHT, Wing& inVT, TurboFan& inEngine, Nacelle& inNacelle, Fuselage& inFuselage, double inFuelWeight, double inPayLoadWeight);

		double calcMach(double velocity, double temp) const;

		double calcDragCoeff(double AoA, double velocity, double Mach, double kinematicViscosity) const;
		double calcDrag(double AoA, double velocity, double Mach, double kinematicViscosity, double density) const;

		double calcLiftCoeff(double AoA) const;
		double calcLift(double AoA, double velocity, double density) const;

		// Climb Functions
		double calcBestClimbTime(double startHeight, double endHeight) const;               // "Best" means we somehow get to the V we need to get to instaneously
		double calcSteadyClimbAoA(double gamma, double velocity, double density) const;     // Gamma in degrees, Returns AoA in rad




		// Accessors:
		double getWeight() const;

	private:
		double totalWeight;      // Class calculates this
		double referenceArea;    // Class assigns this the area of the main wing
		Wing* HT;                // Full HT (Full Span / Planform)
		Wing* VT;                // Full VT (Full Span / Planform)
		Wing* mainWing;          // Full mainWing (Full Span / Planform)
		TurboFan* engine;        // One engine (Class assumes plane has 2 identical engines)
		Nacelle* nacelle;        // One nacelle (Class assumes plane has 2 identical nacelles)
		Fuselage* fuselage;      // One fuselage
		double payLoadWeight;    //
		double fuelWeight;       // Weight of Fuel
		LiftCoeff CL;           // One CL for the whole plane
		static constexpr double GAS_CONSTANT = 1716;
		static constexpr double pi = 3.141592653589;



		// computeAndSet Functions:
		void computeAndSetTotalWeight();
		void computeAndSetLiftCoeff();													 // Needed for SteadyClimbAoA... sets Cl


		// Climb Functions
		double calcMaxExcessPower(double startHeight, double endHeight) const;   // Prob also need max excess power speed
	};




}


#endif