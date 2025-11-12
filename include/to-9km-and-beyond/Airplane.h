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

		double calcMach(double velocity, double temp) const;


		//double calculateLift();
		//double calculateDrag();

	
		// Accessors:
		double getWeight() const;

	private:
		// member variables
		double totalWeight;

		double payLoadWeight;     
		double fuelWeight;
		static constexpr double GAS_CONSTANT = 1716;


		Wing* HT;                // One Planform of Horizontal Tail (one side)
		Wing* VT;                // One Planform of Vertical Tail (one side)
		Wing* mainWing;          // One Planform of Main Wing (one side)
		TurboFan* engine;        // Assumption that 2 engines are both with same propertities and that they are turbofans
		Nacelle* nacelle;        // Assumption that 2 nacelles, both with same propertities
		Fuselage* fuselage; 



		double calcTotalWeight() const;
	};




}


#endif