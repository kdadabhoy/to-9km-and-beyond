#include <iostream>
#include "to-9km-and-beyond/Airfoil.h"
#include <string>
#include <cassert>

namespace airplane {
	
	Airfoil::Airfoil() {
		NACA = "0000";
		maxCamber = 0;
		posMaxCamber = 0;
		maxThickness = 0;

	}


	Airfoil::Airfoil(const std::string& inNACA){
		assert(inNACA.size() == 4);
		NACA = inNACA;

		maxCamber = std::stoi(NACA.substr(0, 1)) / 100.0;
		posMaxCamber = std::stoi(NACA.substr(1, 1)) / 10.0;
		maxThickness = std::stoi(NACA.substr(2, 2)) / 100.0;
		Cl = calcCl();
	}




	Airfoil::Airfoil(const std::string& inNACA, const LiftCoeff& inCl, double inAlphaZeroLift) {
		assert(inNACA.size() == 4);
		NACA = inNACA;
		Cl = inCl;
		alphaZeroLift = inAlphaZeroLift;

		maxCamber = std::stoi(NACA.substr(0, 1)) / 100.0;
		posMaxCamber = std::stoi(NACA.substr(1, 1)) / 10.0;
		maxThickness = std::stoi(NACA.substr(2, 2)) / 100.0;

	}








	// This seems to be a made up approx... Look up thin airfoil theory and prob write an integration thing to calculate this
	// Verify below function
	// Kind of irrelevant bc I dont really care about C0
	LiftCoeff Airfoil::calcCl() const {
		double C0;
		double alphaTerm = 2*pi;														// Cl_alpha = 2pi for thin-airfoil theory approx        

		if (posMaxCamber == 0) {
			C0 = 0;
		} else {
			C0 = (-2.0 * maxCamber / posMaxCamber) * (1 - (posMaxCamber / 2));          // radians, thin-airfoil theory approx
		}

		LiftCoeff tempCoeff(alphaTerm, C0);
		return tempCoeff;
	}


	// ^ Verify above funciton


	// Need to figure out math
	double Airfoil::calcalphaZeroLift() const {
		return 0;
	}


	// ^ Verify above funciton



	// Helpful Functions
	double Airfoil::getCl_deg(double inAlphaDeg) {
		double alphaRad = inAlphaDeg * (pi / 180);
		return Cl.get_CL_rad(alphaRad);
	}



	double Airfoil::getCl_rad(double inAlphaRad) {
		return Cl.get_CL_rad(inAlphaRad);
	}




	// Accessors:
	double Airfoil::getCl_AlphaTerm() {
		return Cl.getCL_Alpha();
	}
	
	double Airfoil::getCl_alphaZeroLift() {
		return alphaZeroLift;
	}









}