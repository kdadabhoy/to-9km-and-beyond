#include <iostream>
#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/Airfoil.h"
#include "to-9km-and-beyond/LiftCoeff.h"
#include <cmath>
#include <cassert>
using namespace std;


namespace airplane {
	// Default Constructor
	Wing::Wing() {
		airfoil = nullptr;
		tipChord = 0;
		rootChord = 0;
		sweepAngle = 0;
		span = 0;
		taperRatio = 0;
		area = 0;
		MAC = 0;
		aspectRatio = 0;
		weight = 0;
		ellipEfficiency = 0;
	}



	// main wing (so it calcs weight for me)
	Wing::Wing(Airfoil& inAirfoil, double inSpan, double inTipChord, double inRootChord, double inSweepAngle) {
		tipChord = inTipChord / 12;
		rootChord = inRootChord / 12;
		sweepAngle = inSweepAngle;
		span = inSpan / 12;
		airfoil = &inAirfoil;

		taperRatio = inTipChord / inRootChord;

		area = calcArea(taperRatio);
		MAC = calcMAC(taperRatio);
		aspectRatio = calcAspectRatio(area);
		ellipEfficiency = calcEllipEfficiency(aspectRatio);
		calcCL3D();

		weight = calcWeight();
	}


	// HT and VT (so I can have weight = 0)
	Wing::Wing(Airfoil& inAirfoil, double inSpan, double inTipChord, double inRootChord, double inSweepAngle, double inWeight) {
		tipChord = inTipChord / 12;
		rootChord = inRootChord / 12;
		sweepAngle = inSweepAngle;
		span = inSpan / 12;
		airfoil = &inAirfoil;

		taperRatio = inTipChord / inRootChord;

		area = calcArea(taperRatio);
		MAC = calcMAC(taperRatio);
		aspectRatio = calcAspectRatio(area);
		calcCL3D();

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
		ellipEfficiency = other.ellipEfficiency;
		airfoil = other.airfoil;					// This could run into pointer errors... and prob needs dynamic memory allocation...
													// But won't really be using a copy constructor so... :)
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

	double Wing::calcEllipEfficiency(const double inAspectRatio) const {
		double effic;
		effic = 1.78 * (1 - (.045 * pow(inAspectRatio, 0.68))) - 0.65;
		return effic;
	}





	//Weight Calc

	double Wing::calcWeight() const {
		double weight = -1;
		// Need to figure out approx
		return weight;
	}



	// CL 3D Calc
	void Wing::calcCL3D() {
		double Cl2D_alphaTerm = airfoil->getCl_AlphaTerm();						// This is gunna be 2pi always
		double Cl2D_alphaZeroLift = airfoil->getCl_alphaZeroLift();				// This differs by airfoil
		double knottTerm;
		double alphaTerm;
		double a0 = 0;
		double radSweepAngle = sweepAngle * pi / 180;

		//a0 = Cl2D_alphaTerm / (1 + (Cl2D_alphaTerm / (pi * ellipEfficiency * aspectRatio)));      // Different formula that uses efficiency

		assert(sweepAngle <= 89.0);
		a0 = (pi * aspectRatio) / (1 + sqrt(1 + pow((pi * aspectRatio) / (Cl2D_alphaTerm * cos(radSweepAngle)), 2)));        // Accounts for Sweep Angle

		knottTerm = Cl2D_alphaZeroLift * a0;
		alphaTerm = a0;

		CL3D.setCL_Alpha(alphaTerm);
		CL3D.setCL_Knott(knottTerm);
	}



	// Semi-Related Calculations

	double Wing::calcReynolds(const double velocity, const double kinematicViscosity) const {

		return ((velocity * MAC) / kinematicViscosity);
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
		return ellipEfficiency;
	}

	double Wing::getC_L_rad(const double AoA) const {
		return CL3D.get_CL_rad(AoA);
	}


}