#include <iostream>
#include "to-9km-and-beyond/Wing.h"
using namespace std;

namespace airplane {
	// Default Constructor
	Wing::Wing() {
		tipChord = 0;
		rootChord = 0;
		sweepAngle = 0;
		span = 0;
		taperRatio = 0;
		area = 0;
		MAC = 0;
		aspectRatio = 0;
		weight = 0;
	}


	// main wing (so it calcs weight for me)
	Wing::Wing(double inSpan, double inTipChord, double inRootChord, double inSweepAngle) {
		tipChord = inTipChord / 12;
		rootChord = inRootChord / 12;
		sweepAngle = inSweepAngle;
		span = inSpan / 12;

		taperRatio = inTipChord / inRootChord;

		area = calcArea(taperRatio);
		MAC = calcMAC(taperRatio);
		aspectRatio = calcAspectRatio(area);

		weight = calcWeight();
	}


	// HT and VT (so I can have weight = 0)
	Wing::Wing(double inSpan, double inTipChord, double inRootChord, double inSweepAngle, double inWeight) {
		tipChord = inTipChord / 12;
		rootChord = inRootChord / 12;
		sweepAngle = inSweepAngle;
		span = inSpan / 12;

		taperRatio = inTipChord / inRootChord;

		area = calcArea(taperRatio);
		MAC = calcMAC(taperRatio);
		aspectRatio = calcAspectRatio(area);

		weight = inWeight;
	}



	//Copy Constructor
	Wing::Wing(const Wing& other) {
		tipChord = other.tipChord;
		rootChord = other.rootChord;
		sweepAngle = other.sweepAngle;
		span = other.span;
		taperRatio = other.taperRatio;
		area = other.area;
		MAC = other.MAC;
		aspectRatio = other.aspectRatio;
		weight = other.weight;
	}


	double Wing::calcArea(const double inTaperRatio) const {
		return (0.5 * rootChord * span * (inTaperRatio + 1));
	}


	double Wing::calcMAC(const double inTaperRatio) const {
		return (2.0/3.0) * rootChord * ((1 + inTaperRatio + (inTaperRatio * inTaperRatio)) / (1 + inTaperRatio));
	}


	double Wing::calcAspectRatio(const double inArea) const {
		return span * span / inArea;
	}





	//Weight Calc

	double Wing::calcWeight() const {
		double weight = -1;
		// Need to figure out approx
		return weight;
	}





	// Semi-Related Calculations

	double Wing::calcReynolds(const double velocity, const double kinematicViscosity) const {

		return velocity * MAC / kinematicViscosity;
	}




	double Wing::calcWettedArea() const {
		return 2 * area * 1.02;
	}




	// Accessors:
	double Wing::getArea() const {
		return area;
	}

	double Wing::getMAC() const {
		return MAC;
	}

	double Wing::getTaperRatio() const {
		return taperRatio;
	}

	double Wing::getAspectRatio() const {
		return aspectRatio;
	}

	double Wing::getWeight() const {
		return weight;
	}

	double Wing::getAR() const {
		return aspectRatio;
	}

	double Wing::getEllipticalEffic() const {
		return .8;
	}
	double Wing::getC_L(const double AoA) const {
		return 1 * AoA;
	}



}