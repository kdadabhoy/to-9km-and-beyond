#include <iostream>
#include "to-9km-and-beyond/TurboFan.h"
using namespace std;


namespace airplane {

	TurboFan::TurboFan() {
		weight = 0;
		SLSThrust = 0;
		specificFuelConsumption = 0;
	}




	TurboFan::TurboFan(double inWeight, double inSLSThrust) {
		weight = inWeight;
		SLSThrust = inSLSThrust;
		specificFuelConsumption = .5;  // should prob take input
	}





	// Copy constrcutor
	TurboFan::TurboFan(const TurboFan& other) {
		weight = other.weight;
		SLSThrust = other.SLSThrust;
		specificFuelConsumption = other.specificFuelConsumption;
	}




	// Accessors:
	double TurboFan::getWeight() const {
		return weight;
	}

	double TurboFan::getSLSThrust() const {
		return SLSThrust;
	}


	double TurboFan::getSpecificFuelConsumption() const {
		return specificFuelConsumption;
	}






	// Mutators:
	void TurboFan::setWeight(double inWeight) {
		weight = inWeight;
	}


	void TurboFan::setSLSThrust(double inThrust) {
		SLSThrust = inThrust;
	}



	void TurboFan::setSpecificFuelConsumption(double inFrac) {
		specificFuelConsumption = inFrac;
	}

}
