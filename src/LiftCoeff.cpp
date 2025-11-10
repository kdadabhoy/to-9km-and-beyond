#include <iostream>
#include "to-9km-and-beyond/LiftCoeff.h"

using namespace std;

namespace airplane {
	LiftCoeff::LiftCoeff() {
		CL_alpha = 0;
		CL_knott = 0;
	}




	LiftCoeff::LiftCoeff(double inCL_alpha, double inCL_knott) {
		CL_alpha = inCL_alpha;
		CL_knott = inCL_knott;
	}



	// Copy Constructor
	LiftCoeff::LiftCoeff(LiftCoeff& other) {
		CL_alpha = other.CL_alpha;
		CL_knott = other.CL_knott;
	}



	// Assignment Operator
	LiftCoeff& LiftCoeff::operator=(const LiftCoeff& other) {
		if (this == &other) {
			return *this;
		} else {
			CL_alpha = other.CL_alpha;
			CL_knott = other.CL_knott;
			return *this;
		}
	}





	// Accessors:
	double LiftCoeff::getCL_Alpha() const {
		return CL_alpha;
	}

	double LiftCoeff::getCL_Knott() const {
		return CL_knott;
	}





	// Mutators:
	void LiftCoeff::setCL_Alpha(double inTerm) {
		CL_alpha = inTerm;
	}


	void LiftCoeff::setCL_Knott(double inTerm) {
		CL_knott = inTerm;
	}




	// Useful Functions:
	double LiftCoeff::get_CL_rad(double alpha) const {
		return (CL_alpha * alpha) + CL_knott;
	}




}