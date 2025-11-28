#include <iostream>
#include "to-9km-and-beyond/DragCoeff.h"
#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/Nacelle.h"
#include "to-9km-and-beyond/Fuselage.h"
#include <cmath>
#include <cassert>
using namespace std;




namespace airplane {
	DragCoeff::DragCoeff() {
		Wing = nullptr;
		Fuselage = nullptr;
		Nacelle = nullptr;
	}









	DragCoeff::DragCoeff(const airplane::Wing& inWing) {
		Wing = &inWing;
		Fuselage = nullptr;
		Nacelle = nullptr;
	}









	DragCoeff::DragCoeff(const airplane::Fuselage& inFuselage) {
		Fuselage = &inFuselage;
		Wing = nullptr;
		Nacelle = nullptr;
	}









	DragCoeff::DragCoeff(const airplane::Nacelle& inNacelle) {
		Nacelle = &inNacelle;
		Wing = nullptr;
		Fuselage = nullptr;
	}









	double DragCoeff::calcTotalDragCoeff(double AoA, double Reynolds, double Mach, double wetAreaRatio) const {
		// Induced Drag AoA can be negative.. and should be (Bc CL will be off it it isnt... it still outputs pos value bc CL^2)
		// InducedCoeff AoA can be negative... the Wing member functions can handle negative numbers
			// These helpfer functions take fabs(AoA) bc graph is only defined for CL>0...
			// In theory this is fine as long as CL is evalulated at negative AoA, then the magnitude is taken
		if (Wing) {
			return calcParasiteCoeff(Reynolds, wetAreaRatio) + calcCompressibilityCoeff(Mach, AoA) + calcInducedCoeff(AoA);
		} else if (Fuselage) {
			double parasiteTemp;
			parasiteTemp = calcParasiteCoeff(Reynolds, wetAreaRatio);
			return parasiteTemp + calcFormDragCoeff(parasiteTemp);
		} else if (Nacelle) {
			double parasiteTemp;
			parasiteTemp = calcParasiteCoeff(Reynolds, wetAreaRatio);
			return parasiteTemp + calcFormDragCoeff(parasiteTemp);
		} else {
			return 0;
		}
	}










	double DragCoeff::calcParasiteCoeff(double Reynolds, double wetAreaRatio) const {
		assert(Wing != nullptr || Fuselage != nullptr || Nacelle != nullptr);

		if (Reynolds >= CRITICAL_REYNOLDS_NUMBER) {
			return wetAreaRatio * .455 / pow(log10(Reynolds), 2.58);
		} else if (Reynolds > .001) {
			return wetAreaRatio * 1.328 / sqrt(Reynolds);
		} else {
			return 0;
		}
	}









	double DragCoeff::calcInducedCoeff(double AoA) const {
		assert(Wing != nullptr);
		double CL = Wing->calcLiftCoeff(AoA);
		return (CL*CL) / (pi * Wing->getEllipticalEffic() * Wing->getAspectRatio());
	}









	double DragCoeff::calcCompressibilityCoeff(double Mach, double AoA) const {
		assert(Wing != nullptr);

		if (Mach < .3) {
			// Compressibility drag negligble 
			return 0;
		} 


		double machRatio = Mach / Wing->calcMcc(AoA);      // M_freestream / Mcc

		if (machRatio < .75) {
			// Assuming compressibility negligible bc Shevell graph doesn't cover it
			return 0;
		} else {
			assert(machRatio >= .75 && machRatio <= 1.5);        // This is extrapolating to 1.5... which is an assumption
																 // This assumption is needed in order to produce an accurate powerCurve
																 // If you don't extrapolate you get odd curves.. well rather we abort the program
			double Cdc = 0;
			double sweepCosine = cos(Wing->getSweepAngleRad());

			// y = (7.05*10^-11)*e^(17x)
			Cdc = (7.05e-11) * exp(17 * machRatio);
			Cdc = Cdc / (sweepCosine * sweepCosine * sweepCosine);
			return Cdc;
		}
	}









	// Form Drag - Fuselage and Nacelle Only
	double DragCoeff::calcFormDragCoeff(double Cf) const {
		assert(Fuselage != nullptr || Nacelle != nullptr);
		if (Fuselage) {
			return Cf * Fuselage->getFormFactor();
		} else if (Nacelle) {
			return Cf * Nacelle->getFormFactor();
		} else {
			return 0;
		}
	}


}
