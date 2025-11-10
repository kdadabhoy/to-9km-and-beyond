#include <iostream>
#include "to-9km-and-beyond/DragCoeff.h"
#include "to-9km-and-beyond/Wing.h"
#include <cmath>
#include <cassert>
using namespace std;



namespace airplane {
	DragCoeff::DragCoeff() {
		parasiteCoeff = 0;
		inducedCoeff = 0;
		compressibilityCoeff = 0;
		Wing = nullptr;
	}
	



	DragCoeff::DragCoeff(airplane::Wing& inWing) {
		Wing = &inWing;
		parasiteCoeff = calcParasiteCoeff(0, 1.5723e-4);
		inducedCoeff = calcInducedCoeff(0);
		compressibilityCoeff = 0;
	}




                 
	// Total Drag Functions 

	// Need
	// AoA,							or CL
	// kinematicViscosity			or Reynolds	
	// temp                         or Mach
	// velocity    

	// Prob also need Mach Critical Crest (have a wing function for this

	// Total Drag Functions:

	// Most efficient
	double DragCoeff::calcTotalDragCoeff(double AoA, double Reynolds, double Mach, double wetAreaRatio) const {
		return calcParasiteCoeff(Reynolds, wetAreaRatio) + calcCompressibilityCoeff(Mach) + calcInducedCoeff(AoA);

	}



	// Older one - works best for outside Wing Class implementations (?), honestly prob won't use
	double DragCoeff::calcTotalDragCoeff(double AoA, double velocity, double kinematicViscosity, double temp, double wetAreaRatio) const {
		double Mach = velocity / sqrt(1.4 * GAS_CONSTANT * temp);
		return calcParasiteCoeff(velocity, kinematicViscosity, wetAreaRatio) + calcCompressibilityCoeff(Mach) + calcInducedCoeff(AoA);
	}









	// Parasite Drag Functions:
	
	// Most efficient
	double DragCoeff::calcParasiteCoeff(double Reynolds, double wetAreaRatio) const {
		assert(Wing != nullptr);

		if (Reynolds >= 5e5) {
			return wetAreaRatio * .455 / pow(log10(Reynolds), 2.58);
		} else if (Reynolds > .001) {
			return wetAreaRatio * 1.328 / sqrt(Reynolds);
		} else {
			return 0;
		}
	}





	// Older one
	double DragCoeff::calcParasiteCoeff(double velocity, double kinematicViscosity, double wetAreaRatio) const {
		assert(Wing != nullptr);
		double Re = Wing->calcReynolds(velocity, kinematicViscosity);

		if (Re >= 5e5) {
			return wetAreaRatio * .455 / pow(log10(Re), 2.58);
		} else if (Re > .001) {
			return wetAreaRatio * 1.328 / sqrt(Re);
		} else {
			return 0;
		}

	}





	// Induced Drag Functions:
	double DragCoeff::calcInducedCoeff(double AoA) const {
		assert(Wing != nullptr);
		double CL = Wing->getC_L_rad(AoA);
		return (CL*CL) / (pi * Wing->getEllipticalEffic() * Wing->getAspectRatio());
	}





	// Induced Compressibility Functions:
	double DragCoeff::calcCompressibilityCoeff(double Mach) const {
		if (Mach < .30) {
			return 0;		// Keep as 0 bc negligble
		} else {
			// Need to implement digitized CDC graph
			return 0;
		}
	}




}