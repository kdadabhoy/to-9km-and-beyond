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





                 

	// Total Drag Functions 

	// Need
	// AoA,							or CL
	// kinematicViscosity			or Reynolds	
	// temp                         or Mach
	// velocity    

	// Prob also need Mach Critical Crest (have a wing function for this



	// Total Drag Function
	double DragCoeff::calcTotalDragCoeff(double AoA, double Reynolds, double Mach, double wetAreaRatio) const {
		if (Wing) {
			return calcParasiteCoeff(Reynolds, wetAreaRatio) + calcCompressibilityCoeff(Mach) + calcInducedCoeff(AoA);
		} else if (Fuselage) {
			double parasiteTemp;
			parasiteTemp = calcParasiteCoeff(Reynolds, wetAreaRatio);
			return parasiteTemp + calcFormDragCoeff(parasiteTemp);
		} else {
			return 0;
		}
	}





	// Parasite Drag Function
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




	// Induced Drag Function
	double DragCoeff::calcInducedCoeff(double AoA) const {
		assert(Wing != nullptr);
		double CL = Wing->getC_L_rad(AoA);
		return (CL*CL) / (pi * Wing->getEllipticalEffic() * Wing->getAspectRatio());
	}





	// Induced Compressibility Functions
	double DragCoeff::calcCompressibilityCoeff(double Mach) const {
		assert(Wing != nullptr);

		if (Mach < .30) {
			return 0;		// Keep as 0 bc negligble
		} else {
			// Need to implement digitized CDC graph
			return 0;
		}
	}




	// Form Drag - Fuselage Only
	double DragCoeff::calcFormDragCoeff(double Cf) const {
		assert(Fuselage != nullptr);
		return Cf * Fuselage->getFormFactor();
	}




}