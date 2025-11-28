#include <iostream>
#include "to-9km-and-beyond/Nacelle.h"
#include "to-9km-and-beyond/DragCoeff.h"
#include <cassert>
using namespace std;




namespace airplane {
	Nacelle::Nacelle() {
		weight = 0;
		length = 0;
		wettedArea = 0;
	}









	// Takes in inches, but stores privately in ft
	Nacelle::Nacelle(double inWeight, double inLength, double inWettedArea) {
		weight = inWeight;
		length = inLength / 12;
		wettedArea = inWettedArea / 12;
	}










	// Copy constrcutor
	Nacelle::Nacelle(const Nacelle& other) {
		weight = other.weight;
		length = other.length;
		wettedArea = other.wettedArea;
		formFactor = other.formFactor;
	}










	// Assignment Operator
	Nacelle& Nacelle::operator=(const Nacelle& other) {
		if (this == &other) {
			return *this;
		} else {
			weight = other.weight;
			length = other.length;
			wettedArea = other.wettedArea;
			formFactor = other.formFactor;
			return *this;
		}
	}











	// Useful Functions:
	double Nacelle::calcReynolds(double velocity, double kinematicViscosity) const {
		return ((velocity * length) / kinematicViscosity);
	}











	double Nacelle::calcWetRatio(double referenceArea) const {
		// WetArea divided by referenceArea
		return  wettedArea / referenceArea;
	}










	double Nacelle::calcDragCoeff(double AoA, double Reynolds, double Mach, double wetAreaRatio) const {
		assert(Mach <= .98);
		DragCoeff CD(*this);
		return CD.calcTotalDragCoeff(AoA, Reynolds, Mach, wetAreaRatio);
	}










	// Mutators:
	void Nacelle::setWeight(double inWeight) {
		weight = inWeight;
	}








	// Accessors:
	double Nacelle::getWeight() const {
		return weight;
	}

	double Nacelle::getFormFactor() const {
		return formFactor;
	}

	double Nacelle::getWettedArea() const {
		return wettedArea;
	}





}
