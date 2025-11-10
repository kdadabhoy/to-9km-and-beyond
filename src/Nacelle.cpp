#include <iostream>
#include "to-9km-and-beyond/Nacelle.h"
using namespace std;
// Prob should add assignment operator at some point



namespace airplane {

	Nacelle::Nacelle() {
		weight = 0;
	}
	Nacelle::Nacelle(double inWeight) {
		weight = inWeight;
	}

	// Copy constrcutor
	Nacelle::Nacelle(const Nacelle& other) {
		weight = other.weight;
	}

	Nacelle& Nacelle::operator=(const Nacelle& other) {
		if (this == &other) {
			return *this;
		} else {
			weight = other.weight;
			return *this;
		}
	}


	double Nacelle::getWeight() const {
		return weight;
	}

	void Nacelle::setWeight(double inWeight) {
		weight = inWeight;
	}
}
