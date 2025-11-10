#include <iostream>
#include "to-9km-and-beyond/Airplane.h"
#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/TurboFan.h"
#include "to-9km-and-beyond/Nacelle.h"
#include <cassert>
#include <cmath>
using namespace std;

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
		fuelWeight = inFuelWeight;         // lbm
		payLoadWeight = inPayLoadWeight;     // lbm
		totalWeight = calcTotalWeight();
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









}