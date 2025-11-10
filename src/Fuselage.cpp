#include <iostream>
#include "to-9km-and-beyond/Fuselage.h"
using namespace std;

namespace airplane {

	Fuselage::Fuselage() {
		weight = 0;
	}
	Fuselage::Fuselage(double inWeight) {
		weight = inWeight;
	}

	// Copy constrcutor
	Fuselage::Fuselage(const Fuselage& other) {
		weight = other.weight;
	}


	double Fuselage::getWeight() const {
		return weight;
	}

	void Fuselage::setWeight(double inWeight) {
		weight = inWeight;
	}
}
