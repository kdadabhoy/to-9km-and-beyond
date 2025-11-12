#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/TurboFan.h"
#include "to-9km-and-beyond/Nacelle.h"
#include "to-9km-and-beyond/Fuselage.h"


// Assumes that plane is symmetric and has 2 engines (wings are symmetrical)
// Assumes VT makes negligible lift a 

namespace airplane {
	class Airplane {
	public:
		Airplane();
		Airplane(Wing& inWing, Wing& inHT, Wing& inVT, TurboFan& inEngine, Nacelle& inNacelle, Fuselage& inFuselage, double inFuelWeight, double inPayLoadWeight);

		// Useful Functions
		double calcMach(double velocity, double temp) const;
		double calcDragCoeff(double AoA, double velocity, double Mach, double kinematicViscosity) const;
		double calcLiftCoeff(double AoA) const;


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
		static constexpr double GAS_CONSTANT = 1716;

		double calcTotalWeight() const;

	};




}


#endif