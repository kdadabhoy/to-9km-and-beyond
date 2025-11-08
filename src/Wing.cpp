#include <iostream>
#include "to-9km-and-beyond/Wing.h"
using namespace std;

namespace Airplane {
	// Default Constructor
	Wing::Wing() {
		tipChord = -1;
		rootChord = -1;
		sweepAngle = -1;
		span = -1;
		taperRatio = -1;
		area = -1;
		MAC = -1;
		aspectRatio = -1;
		weight = -1;
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







	double Wing::calcWeight() const {
		double weight = -1;
		// Need to figure out approx
		return weight;
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





}