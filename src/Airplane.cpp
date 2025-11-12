#include <iostream>
#include "to-9km-and-beyond/Airplane.h"
#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/TurboFan.h"
#include "to-9km-and-beyond/Nacelle.h"
#include <cassert>
#include <cmath>
using namespace std;

// Assumes we are passing in the full span of the Wing and HT (in other words they are symmetrical)
	// In other, other words, the Wing objects represent the full main wing and full HT 
// Prob should add assignment operator at some point


namespace airplane {

	Airplane::Airplane() {
		mainWing = nullptr;
		HT = nullptr;
		VT = nullptr;
		engine = nullptr;
		nacelle = nullptr;
		fuselage = nullptr;
		fuelWeight = 0; 
		payLoadWeight = 0; 
		referenceArea = 0;
		totalWeight = calcTotalWeight();
	}








	// don't do const Class& object, because I might want to modify the object in this class...
	// If I decide none of the objects get modified in this class, then add const here and the pointer won't
	// allow modificaiton to the passed object
	// Would also have to change all member vairbales to const Class*
	Airplane::Airplane(Wing& inWing, Wing& inHT, Wing& inVT, TurboFan& inEngine, Nacelle& inNacelle, Fuselage& inFuselage, double inFuelWeight, double inPayLoadWeight){
		mainWing = &inWing;
		HT = &inHT;
		VT = &inVT;
		engine = &inEngine;
		nacelle = &inNacelle;
		fuselage = &inFuselage;
		fuelWeight = inFuelWeight;
		payLoadWeight = inPayLoadWeight; 
		totalWeight = calcTotalWeight();
		assert(mainWing != nullptr);
		referenceArea = mainWing->getArea();
	}





	double Airplane::calcTotalWeight() const {
		double total = 0.0;

		if (mainWing) {
			total += mainWing->getWeight();
		}

		if (HT) {
			total += (HT->getWeight() * 2);
		}

		if (VT) {
			total += VT->getWeight();
		}

		if (engine) {
			total += (engine->getWeight() * 2);
		}

		if (nacelle) {
			total += (nacelle->getWeight() * 2);
		}

		if (fuselage) {
			total += fuselage->getWeight();
		}

		return total + fuelWeight + payLoadWeight;
	}



	// Accessor:
	double Airplane::getWeight() const {
		return totalWeight;
	}


	// Useful Functions:

	double Airplane::calcMach(double velocity, double temp) const {
		return velocity / sqrt(1.4 * GAS_CONSTANT * temp);
	}




	double Airplane::calcDragCoeff(double AoA, double velocity, double Mach, double kinematicViscosity) const {
		double totalDrag = 0;

		totalDrag += mainWing->calcDragCoeff(AoA, mainWing->calcReynolds(velocity, kinematicViscosity), Mach, mainWing->calcWetRatio(referenceArea));
		totalDrag += HT->calcDragCoeff(AoA, HT->calcReynolds(velocity, kinematicViscosity), Mach, HT->calcWetRatio(referenceArea));
		totalDrag += fuselage->calcDragCoeff(AoA, fuselage->calcReynolds(velocity, kinematicViscosity), Mach, fuselage->calcWetRatio(referenceArea));
		// Might need to add drag for VT, but just the parasite drag (inducded drag acts horizontally?)

		return totalDrag;
	}


	double Airplane::calcLiftCoeff(double AoA) const {
		double totalLift = 0;

		totalLift += mainWing->calcLiftCoeff(AoA);
		totalLift += HT->calcLiftCoeff(AoA);
		totalLift += fuselage->calcLiftCoeff(AoA);
		// Assume VT contributes negligble lift.

		return totalLift;
	}


}