#include <iostream>
#include "to-9km-and-beyond/Fuselage.h"
#include "to-9km-and-beyond/DragCoeff.h"
#include <cassert>
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





	//Assignment Operator
	Fuselage& Fuselage::operator=(const Fuselage& other) {
		if (this == &other) {
			return *this;
		} else {
			weight = other.weight;
			return *this;
		}
	}






	//Useful Member functions
	double Fuselage::calcWetRatio(double referenceArea) const {
		// WetArea divided by referenceArea
		return  wettedArea / referenceArea;
	}







	double Fuselage::calcLiftCoeff(double AoA) const {
		return CL_alpha * AoA + CL_knott;
	}




	double Fuselage::calcDragCoeff(double AoA, double Reynolds, double Mach, double wetAreaRatio) const {
		assert(Mach <= .98);
		DragCoeff CD(*this);
		return CD.calcTotalDragCoeff(AoA, Reynolds, Mach, wetAreaRatio);
	}






	double Fuselage::calcReynolds(double velocity, double kinematicViscosity) const {
		return ((velocity * length) / kinematicViscosity);
	}





	// Mutators
	void Fuselage::setWeight(double inWeight) {
		weight = inWeight;
	}





	// Accessors
	double Fuselage::getWeight() const {
		return weight;
	}

	double Fuselage::getFormFactor() const {
		return formFactor;
	}

	double Fuselage::getWettedArea() const {
		return wettedArea;
	}

	double Fuselage::getCL_Alpha() const {
		return CL_alpha;
	}

	double Fuselage::getCL_Knott() const {
		return CL_knott;
	}



}
