#include <iostream>
#include "to-9km-and-beyond/TurboFan.h"
using namespace std;


namespace airplane {

	TurboFan::TurboFan() {
		weight = -1;
	}




	TurboFan::TurboFan(double inWeight) {
		weight = inWeight;
	}





	// Copy constrcutor
	TurboFan::TurboFan(const TurboFan& other) {
		weight = other.weight;
	}






	double TurboFan::getWeight() const {
		return weight;
	}

	void TurboFan::setWeight(double inWeight) {
		weight = inWeight;
	}
}
