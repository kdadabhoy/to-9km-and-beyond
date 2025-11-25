#include <iostream>
#include "to-9km-and-beyond/DragCoeff.h"
#include "to-9km-and-beyond/Wing.h"
#include <cmath>
#include <cassert>
using namespace std;




namespace airplane {
	DragCoeff::DragCoeff() {
		Wing = nullptr;
		Fuselage = nullptr;
	}
	







	DragCoeff::DragCoeff(const airplane::Wing& inWing) {
		Wing = &inWing;
		Fuselage = nullptr;
	}








	DragCoeff::DragCoeff(const airplane::Fuselage& inFuselage) {
		Fuselage = &inFuselage;
		Wing = nullptr;
	}








	double DragCoeff::calcTotalDragCoeff(double AoA, double Reynolds, double Mach, double wetAreaRatio) const {
		if (Wing) {
			return calcParasiteCoeff(Reynolds, wetAreaRatio) + calcCompressibilityCoeff(Mach, AoA) +calcInducedCoeff(AoA);
		} else if (Fuselage) {
			double parasiteTemp;
			parasiteTemp = calcParasiteCoeff(Reynolds, wetAreaRatio);
			return parasiteTemp + calcFormDragCoeff(parasiteTemp);
		} else {
			return 0;
		}
	}








	double DragCoeff::calcParasiteCoeff(double Reynolds, double wetAreaRatio) const {
		assert(Wing != nullptr || Fuselage != nullptr);

		if (Reynolds >= 5e5) {
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
		CL = fabs(CL);   // Need a postive CL otherwise get negative induced drag
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
			assert(machRatio >= .75 && machRatio <= 1.5);
			double Cdc = 0;
			double sweepCosine = cos(Wing->getSweepAngleRad());

			// y = (7.05*10^-11)*e^(17x)
			Cdc = (7.05e-11) * exp(17 * machRatio);
			Cdc = Cdc / (sweepCosine * sweepCosine * sweepCosine);
			return Cdc;
		}
	}








	// Form Drag - Fuselage Only
	double DragCoeff::calcFormDragCoeff(double Cf) const {
		assert(Fuselage != nullptr);
		return Cf * Fuselage->getFormFactor();
	}








}








/*
	Notes:

		// Total Drag Functions

		// Need
		// AoA,							or CL
		// kinematicViscosity			or Reynolds
		// temp                         or Mach
		// velocity

		// Prob also need Mach Critical Crest (have a wing function for this)










		// Eliminating this from Compressibility Function gives a more continous Power Required Curve
		// The Cdc = .025 was messing it up...

	
		} else if (r <= 1.08) {
			assert(r >= .75 && r <= 1.08); // This function only works if r >= .75 or <= 1.08
			double Cdc = 0;
			double sweepCosine = cos(Wing->getSweepAngleRad());

			// y = (7.05*10^-11)*e^(17x)
			Cdc = (7.05e-11) * exp(17 * r);
			Cdc = Cdc / (sweepCosine*sweepCosine*sweepCosine);
			return Cdc;

		} else {
			assert(r > 1.08 && r < 1.5); // If r is above 1.5 there is probably an issue
			double Cdc = 0;
			double sweepCosine = cos(Wing->getSweepAngleRad());

			// y = .025 - An assumption that heavily incentivizes the optimizer not to go to this
			Cdc = .025;		// Adjust if needed
			Cdc = Cdc / (sweepCosine * sweepCosine * sweepCosine);
			return Cdc;
		}



*/
