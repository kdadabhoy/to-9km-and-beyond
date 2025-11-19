#include <iostream>
#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/Airfoil.h"
#include "to-9km-and-beyond/LiftCoeff.h"
#include "to-9km-and-beyond/DragCoeff.h"
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












// Typically used for main wing... because Airplane Class will take care of calculating it's weight
	Wing::Wing(Airfoil& inAirfoil, double inSpanInch, double inTipChordInch, double inRootChordInch, double inSweepAngleDeg) {
		tipChord = inTipChordInch / 12;
		rootChord = inRootChordInch / 12;
		sweepAngle = inSweepAngleDeg * pi / 180;
		span = inSpanInch / 12;
		airfoil = &inAirfoil;
		taperRatio = inTipChordInch / inRootChordInch;

		// If this is the mainWing, the weight will be set when Airplane constructor is called
		// This is becasue this weight is approxed using MTOW of the Airplane... no (valid) way to calc it here
		weight = 0; 

		calcAndSetAllProperties();
	}













// Typically used for HT and VT because you need to set their weights
	Wing::Wing(Airfoil& inAirfoil, double inSpanInch, double inTipChordInch, double inRootChordInch, double inSweepAngleDeg, double inWeight) {
		tipChord = inTipChordInch / 12;
		rootChord = inRootChordInch / 12;
		sweepAngle = inSweepAngleDeg * pi / 180;
		span = inSpanInch / 12;
		airfoil = &inAirfoil;
		taperRatio = inTipChordInch / inRootChordInch;
		weight = inWeight;

		calcAndSetAllProperties();
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












// CalcAndSet functions:

	// The order these functions are called is very important!
	void Wing::calcAndSetAllProperties() {
		calcAndSetArea();					// Needs span, rootChord, and taperRatio to be defined
		calcAndSetMAC();					// Needs rootChord and taperRatio to be defined
		calcAndSetAspectRatio();			// Needs span and area to be defined
		calcAndSetEllipEfficiency();        // Needs sweepAngle, aspectRatio, and taperRatio to be defined
		calcAndSetCL3D();                   // Needs sweepAngle, aspectRatio, and airfoil to be defined
		return;
	}








	void Wing::calcAndSetArea() {
		area = (0.5 * rootChord * span * (taperRatio + 1));
		return;
	}








	void Wing::calcAndSetMAC() {
		MAC = (2.0 / 3.0) * rootChord * ((1 + taperRatio + (taperRatio * taperRatio)) / (1 + taperRatio));
		return;
	}








	void Wing::calcAndSetAspectRatio() {
		aspectRatio = (span * span) / area;
		return;
	}








	void Wing::calcAndSetEllipEfficiency() {
		double effic;
		double leadingEdgeSweep = 0;
		leadingEdgeSweep = atan(tan(sweepAngle) + ((.25 * (1 - taperRatio)) / (aspectRatio * (1 + taperRatio))));

		if (leadingEdgeSweep < .001 && leadingEdgeSweep > -.001 && aspectRatio > 3.9 && aspectRatio < 10.1) {
			// Emperical formula for unswept Wings
			effic = (1.78 * (1 - (.045 * pow(aspectRatio, 0.68)))) - 0.65;
			assert(effic <= 1 && effic >= .65);
			ellipEfficiency = effic;
			return;
		} else {
			// Good assumption if none of the others apply
			effic = .8;
			ellipEfficiency = effic;
			return;
		}


		/*
			// This is the emperical equation for swept wings with LE sweep > 30
			// but it really only gives e below .7 (and sometimes much lower)
			// which doesn't seem accurate

		else if (leadingEdgeSweep > (29 * pi / 180) && leadingEdgeSweep < (46 * pi / 180) && aspectRatio > 5.9 && aspectRatio < 11.1) {
			// Emperical formula for Swept Wings w/ LE angle > 29
			effic = (4.61 * (1 - (.045 * pow(aspectRatio, 0.68)))) * pow(cos(leadingEdgeSweep), 0.15) - 3.1;
			assert(effic <= 1 && effic >= .65);
			return effic;
		*/
	}















// CL 3D Calc
	void Wing::calcAndSetCL3D() {
		double Cl2D_alphaTerm = airfoil->getCl_AlphaTerm();						// This is gunna be 2pi always
		double Cl2D_alphaZeroLift = airfoil->getCl_alphaZeroLift();				// This differs by airfoil
		double knottTerm;
		double alphaTerm;
		double a0 = 0;

		//a0 = Cl2D_alphaTerm / (1 + (Cl2D_alphaTerm / (pi * ellipEfficiency * aspectRatio)));      // Different formula that uses efficiency

		assert(sweepAngle <= (89.0 * pi / 180));
		double insideSqrtTerm = (pi * aspectRatio) / (Cl2D_alphaTerm * cos(sweepAngle));
		a0 = (pi * aspectRatio) / (1 + sqrt(1 + (insideSqrtTerm * insideSqrtTerm)));        // Accounts for Sweep Angle

		knottTerm = Cl2D_alphaZeroLift * -1 * a0;								            // CL = a0*(alpha - zeroLiftalpha)
		alphaTerm = a0;

		CL3D.setCL_Alpha(alphaTerm);
		CL3D.setCL_Knott(knottTerm);
	}










// Calc Public Functions:

	double Wing::calcArea(double inSpan, double inRootChord, double inTaperRatio) const {
		return (0.5 * inRootChord * inSpan * (inTaperRatio + 1));
	}








	double Wing::calcMAC(double inRootChord, double inTaperRatio) const {
		return (2.0/3.0) * inRootChord * ((1 + inTaperRatio + (inTaperRatio * inTaperRatio)) / (1 + inTaperRatio));
	}








	double Wing::calcAspectRatio(double inSpan, double inArea) const {
		return (inSpan * inSpan) / inArea;
	}








	double Wing::calcEllipEfficiency(double inSweepAngleRad, double inAspectRatio, double inTaperRatio) const {
		double effic;
		double leadingEdgeSweep = 0;
		leadingEdgeSweep = atan(tan(inSweepAngleRad) + ((.25 * (1 - inTaperRatio)) / (inAspectRatio * (1 + inTaperRatio))));

		if (leadingEdgeSweep < .001 && leadingEdgeSweep > -.001 && inAspectRatio > 3.9 && inAspectRatio < 10.1) {
			// Emperical formula for unswept Wings
			effic = (1.78 * (1 - (.045 * pow(inAspectRatio, 0.68)))) - 0.65;
			assert(effic <= 1 && effic >= .65);
			return effic;
		} else {
			// Good assumption if none of the others apply
			return effic = .8;
		}
	}















	// Useful Calcs:

	double Wing::calcReynolds(double velocity, double kinematicViscosity) const {
		return ((velocity * MAC) / kinematicViscosity);
	}








	double Wing::calcWettedArea() const {
		return 2 * area * 1.02;
	}








	double Wing::calcWetRatio(double referenceArea) const {
		// WetArea divided by referenceArea
		return (2 * area * 1.02) / referenceArea;
	}








	double Wing::calcDragCoeff(double AoA_rad, double Reynolds, double Mach, double wetAreaRatio) const {
		assert(Mach <= .99);
		DragCoeff CD_Total(*this);
		return CD_Total.calcTotalDragCoeff(AoA_rad, Reynolds, Mach, wetAreaRatio);
	}








	double Wing::calcLiftCoeff(double AoA_rad) const {
		return CL3D.calcLiftCoefficient(AoA_rad);
	}








	double Wing::calcMcc(double AoA_rad) const {
		// Mcc = Mcc0 / (cos(sweep))^m
		return calcMccZeroSweep(AoA_rad) / pow(cos(sweepAngle), calcSweptMExponent(AoA_rad));
	}








	// Incomplete - Need to finish digitalization
	double Wing::calcMccZeroSweep(double AoA_rad) const {
		double CL = fabs(CL3D.calcLiftCoefficient(AoA_rad));	// NOT SURE IF THIS WORKS, NEEDED BC SOMETIMES HAVE NEGATIVE CL
		double TR = airfoil->getThicknessRatio();           // thickness ratio, called TR for simplicity
		double Mcc = 0;
		double ferror = .01;                                // Error for comparisons of doubles
		assert(CL > 0 && TR > .06 - ferror);

		// Currently implemented with no interpolation (anything in between gets rounded up)
		// which is a conservative approach.. and fairly accurate

		if (CL < ferror) {
			// CL = 0 Case, Mcc = 0.972 + -2.2x + 4.46x^2
			assert(TR > .08 - ferror);							// Might want to comment this out or make a special exception for this case
			Mcc = 0.972 + (-2.2 * TR) + (4.46 * TR * TR);
			return Mcc;
		} else{
			// Very optimistic rn.. bc didnt digitize the rest lol
			Mcc = 0.911 + (-1.48 * TR) + (1.79 * TR * TR);
			return Mcc;
		}



	/*
		//Implement the rest once we digitized the whole plot

		}else if (CL <= .20 + ferror) {
			// CL <= .20 case, Mcc = 0.911 + -1.48x + 1.79x^2
			Mcc = 0.911 + (-1.48 * TR) + (1.79 * TR * TR);
			return Mcc;
		}

		} else if (CL <= .25 + ferror) {
			// CL <=  to .25 case, 

			return Mcc;
		} else if (CL <= .30 + ferror) {
			// CL <=  to .30 case

			return Mcc;
		} else if (CL <= .35 + ferror) {
			// CL <=  to .35 case

			return Mcc;
		} else if (CL <= .40 + ferror) {
			// CL <=  to .40 case

			return Mcc;
		} else if (CL <= .45 + ferror) {
			// CL <=  to .45 case

			return Mcc;
		} else if (CL <= .50 + ferror) {
			// CL <=  to .50 case

			return Mcc;
		} else if (CL <= .55 + ferror) {
			// CL <=  to .55 case

			return Mcc;
		} else if (CL <= .60 + ferror) {
			// CL <= .60 case

			return Mcc;
		} else {
			// Error case
			cout << "CL for Mcc0 > .60. Approx only goes to .60" << endl;
			return 0;
		}

	*/


	}








	double Wing::calcSweptMExponent(double AoA_rad) const {
		double CL = fabs(CL3D.calcLiftCoefficient(AoA_rad));       // AGAIN NOT SURE IF FABS WORKS FOR THIS CL PLOT... but worst case just say we assume symmetric airfoil

		//cout << "CL3d = " << CL << endl; // delete

		if (CL > .6) {
			// Chart doesnt go past CL = .6.. so we will assume anything more is .5
			// This is actually a conservative approx (Bc itll produce a lower Mcc)
			return .5;
		}

		assert(CL > 0 && CL <= .60);							
		//m = 0.823 + -0.57x + 0.101x^2
		return 0.823 + (-.57 * CL) + (0.101 * CL * CL);
	}











// Mutators:
	void Wing::setWeight(double inWeight) {
		weight = inWeight;
		return;
	}









	void Wing::setSpan(double inSpan) {
		span = inSpan / 12;
		// weight = 0;    // Airplane Class fixes weight if it's the main wing
		calcAndSetAllProperties();
		return;
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








	double Wing::getEllipticalEffic() const {
		return ellipEfficiency;
	}








	double Wing::getSweepAngle() const {
		return sweepAngle * 180 / pi;
	}








	double Wing::getSweepAngleRad() const {
		return sweepAngle;
	}








	double Wing::getLeadingEdgeSweep() const {
		return atan(tan(sweepAngle) + ((.25 * (1 - taperRatio)) / (aspectRatio * (1 + taperRatio))));
	}








	double Wing::getCL_Alpha() const {
		return CL3D.getCL_Alpha();
	}








	double Wing::getCL_Knott() const {
		return CL3D.getCL_Knott();
	}








	Airfoil* Wing::getAirfoil() const {
		return airfoil;
	}








}