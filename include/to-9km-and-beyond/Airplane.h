#ifndef AIRPLANE_H
#define AIRPLANE_H

#include "Wing.h"
#include "TurboFan.h"
#include "Nacelle.h"
#include "Fuselage.h"

namespace Airplane {
	class Airplane {
	public:
		Airplane();
		Airplane(Wing inWing, Wing inHT, Wing inVT, TurboFan inEngine, Nacelle inNacelle, Fuselage inFuselage, double inFuelWeight, double inPayLoadWeight);


		double calculateLift();
		double calculateDrag();


	private:
		// memember variables
		double totalWeight;

		double payLoadWeight;     
		double fuelWeight;

		Wing HT;                // One Planform of Horizontal Tail (one side)
		Wing VT;                // One Planform of Vertical Tail (one side)
		Wing mainWing;          // One Planform of Main Wing (one side)
		TurboFan engine;        // Assumption that 2 engines are both with same propertities and that they are turbofans
		Nacelle nacelle;        // Assumption that 2 nacelles, both with same propertities
		Fuselage fuselage; 



		double calcTotalWeight();
	};




}


#endif