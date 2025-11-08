#include <iostream>
#include "to-9km-and-beyond/Airplane.h"
#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/TurboFan.h"
#include "to-9km-and-beyond/Nacelle.h"

using namespace std;

namespace Airplane {

	Airplane::Airplane() {
		mainWing = Wing();
		HT = Wing();
		VT = Wing();
		engine = TurboFan();
		nacelle = Nacelle();
		fuselage = Fuselage();
		fuelWeight = -1; 
		payLoadWeight = -1; 

		totalWeight = calcTotalWeight();
	}

	Airplane::Airplane(Wing inWing, Wing inHT, Wing inVT, TurboFan inEngine, Nacelle inNacelle, Fuselage inFuselage, double inFuelWeight, double inPayLoadWeight){
		mainWing = inWing;
		HT = inHT;
		VT = inVT;
		engine = inEngine;
		nacelle = inNacelle;
		fuselage = inFuselage;
		fuelWeight = inFuelWeight;         // lbm
		payLoadWeight = inPayLoadWeight;     // lbm
		totalWeight = calcTotalWeight();
	}



	double Airplane::calcTotalWeight() {
		return mainWing.getWeight() + HT.getWeight() + VT.getWeight() 
			+ engine.getWeight() + nacelle.getWeight() + fuselage.getWeight() 
			+ fuelWeight + payLoadWeight;
	}














}