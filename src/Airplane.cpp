#include <iostream>
#include "to-9km-and-beyond/Airplane.h"
#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/TurboFan.h"
#include "to-9km-and-beyond/Nacelle.h"

using namespace std;

namespace airplane {

	Airplane::Airplane() {
		mainWing = &Wing();
		HT = &Wing();
		VT = &Wing();
		engine = &TurboFan();
		nacelle = &Nacelle();
		fuselage = &Fuselage();
		fuelWeight = -1; 
		payLoadWeight = -1; 

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
		return mainWing->getWeight() + HT->getWeight() + VT->getWeight()
			+ engine->getWeight() + nacelle->getWeight() + fuselage->getWeight()
			+ fuelWeight + payLoadWeight;
	}














}