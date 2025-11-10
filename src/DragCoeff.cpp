#include <iostream>
#include "to-9km-and-beyond/DragCoeff.h"
#include "to-9km-and-beyond/Wing.h"
#include <cmath>
#include <cassert>
using namespace std;
using namespace airplane;

// Think I have a mach function in atmosphere propertities... so maybe just pass in mach instead



namespace aeroCoeff {
	DragCoeff::DragCoeff() {
		parasiteCoeff = 0;
		inducedCoeff = 0;
		compressibCoeff = 0;
		Wing = nullptr;
		referenceArea = 0;

	}
	



	DragCoeff::DragCoeff(const airplane::Wing& inWing, const double inReferenceArea) {
		Wing = &inWing;
		referenceArea = inReferenceArea;
		parasiteCoeff = calcParasiteCoeff(0, 1.5723e-4);
		inducedCoeff = calcInducedCoeff(0);
	}





	double DragCoeff::calcTotalDragCoeff(const double AoA, const double velocity,
		const double kinematicViscosity, const double temp) 
	{
		int R = 1717;
		double Mach = velocity / sqrt(1.4 * R * temp);
		return calcParasiteCoeff(velocity, kinematicViscosity) + calcCompressibCoeff(Mach) + calcInducedCoeff(AoA);
	}




	double DragCoeff::calcParasiteCoeff(const double velocity, const double kinematicViscosity) {
		assert(Wing != nullptr);
		double Re;
		double areaRatio;

		Re = Wing->calcReynolds(velocity, kinematicViscosity);
		areaRatio = Wing->calcWettedArea() / Wing->getArea();

		if (Re >= 5e5) {
			return areaRatio * .455 / pow(log10(Re), 2.58);
		} else if (Re > .001) {
			return areaRatio * 1.328 / sqrt(Re);
		} else {
			return 0;
		}

	}



	double DragCoeff::calcInducedCoeff(const double AoA) {
		assert(Wing != nullptr);
		double CL = Wing->getC_L_rad(AoA);

		return (CL*CL) / (pi * Wing->getEllipticalEffic() * Wing->getAR());
	}




	double DragCoeff::calcCompressibCoeff(const double Mach) {
		if (Mach < .30) {
			return 0;
		} else {
			// Need to implement digitized CDC graph
			return 1;
		}
	}




}