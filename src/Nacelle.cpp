#include <iostream>
#include "to-9km-and-beyond/Nacelle.h"
using namespace std;

namespace Airplane {

	Nacelle::Nacelle() {
		weight = -1;
	}
	Nacelle::Nacelle(double inWeight) {
		weight = inWeight;
	}

	// Copy constrcutor
	Nacelle::Nacelle(const Nacelle& other) {
		weight = other.weight;
	}


	double Nacelle::getWeight() const {
		return weight;
	}

	void Nacelle::setWeight(double inWeight) {
		weight = inWeight;
	}
}
