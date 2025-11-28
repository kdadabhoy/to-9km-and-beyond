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
		alphaZeroLift = 0;
		Cl.setCL_Alpha(Cl_2D_SLOPE);
		Cl.setCL_Knott(0);
	}








	Airfoil::Airfoil(const std::string& inNACA){
		assert(inNACA.size() == 4);
		NACA = inNACA;

		maxCamber = std::stoi(NACA.substr(0, 1)) / 100.0;
		posMaxCamber = std::stoi(NACA.substr(1, 1)) / 10.0;
		maxThickness = std::stoi(NACA.substr(2, 2)) / 100.0;
		Cl.setCL_Alpha(Cl_2D_SLOPE);
		Cl.setCL_Knott(0);
		alphaZeroLift = calcalphaZeroLift();
	}









	Airfoil::Airfoil(const std::string& inNACA, double inAlphaZeroLift) {
		assert(inNACA.size() == 4);
		NACA = inNACA;
		alphaZeroLift = inAlphaZeroLift;
		Cl.setCL_Alpha(Cl_2D_SLOPE);
		Cl.setCL_Knott(0);

		maxCamber = std::stoi(NACA.substr(0, 1)) / 100.0;
		posMaxCamber = std::stoi(NACA.substr(1, 1)) / 10.0;
		maxThickness = std::stoi(NACA.substr(2, 2)) / 100.0;

	}









	Airfoil::Airfoil(const std::string& inNACA, double inAlphaZeroLift, double inCl0) {
		assert(inNACA.size() == 4);
		NACA = inNACA;
		alphaZeroLift = inAlphaZeroLift;
		Cl.setCL_Alpha(2 * pi);
		Cl.setCL_Knott(inCl0);

		maxCamber = std::stoi(NACA.substr(0, 1)) / 100.0;
		posMaxCamber = std::stoi(NACA.substr(1, 1)) / 10.0;
		maxThickness = std::stoi(NACA.substr(2, 2)) / 100.0;

	}









	// Need to figure out math
	double Airfoil::calcalphaZeroLift() const {
		// Don't think there is a function that allows this
		// Currently we just take in this variable
		return 0;
	}









	// Helpful Functions
	double Airfoil::getCl_deg(double inAlphaDeg) const {
		double alphaRad = inAlphaDeg * (pi / 180);
		return Cl.calcLiftCoefficient(alphaRad);
	}









	double Airfoil::getCl_rad(double inAlphaRad) const {
		return Cl.calcLiftCoefficient(inAlphaRad);
	}









	// Accessors:
	double Airfoil::getCl_AlphaTerm() const {
		return Cl.getCL_Alpha();
	}



	double Airfoil::getCl_alphaZeroLift() const {
		return alphaZeroLift;
	}


	double Airfoil::getCl_KnottTerm() const {
		return Cl.getCL_Knott();
	}



	double Airfoil::getThicknessRatio() const {
		return maxThickness;
	}



	double Airfoil::getMaxCamberRatio() const {
		return maxCamber;
	}



	void Airfoil::printName() const {
		std::cout << "NACA: " << NACA << std::endl;
	}



	std::string Airfoil::getName() const {
		return NACA;
	}




}