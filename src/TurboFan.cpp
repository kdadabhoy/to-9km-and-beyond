#include <iostream>
#include "to-9km-and-beyond/TurboFan.h"
using namespace std;


namespace airplane {

	TurboFan::TurboFan() {
		weight = 0;
		staticThrust = 0;
	}




	TurboFan::TurboFan(double inWeight, double inStaticThrust) {
		weight = inWeight;
		staticThrust = inStaticThrust;
	}





	// Copy constrcutor
	TurboFan::TurboFan(const TurboFan& other) {
		weight = other.weight;
		staticThrust = other.staticThrust;
	}




	// Accessors:
	double TurboFan::getWeight() const {
		return weight;
	}

	double TurboFan::getStaticThrust() const {
		return staticThrust;
	}






	// Mutators:
	void TurboFan::setWeight(double inWeight) {
		weight = inWeight;
	}


	void TurboFan::setStaticThrust(double inThrust) {
		staticThrust = inThrust;
	}
}
