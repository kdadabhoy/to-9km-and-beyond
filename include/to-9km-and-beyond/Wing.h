#ifndef WING_H
#define WING_H
#include "to-9km-and-beyond/Airfoil.h"
#include "to-9km-and-beyond/LiftCoeff.h"
// Prob should add assignment operator at some point
// Can't calculate wing weight in this class because need MTOW

namespace airplane {
	class Wing {
	public:

		Wing();  // Default Constructor
		Wing(Airfoil& inAirfoil, double inSpan, double inTipChord, double inRootChord, double inSweepAngle);  // Takes in inches, used for main wing
		Wing(Airfoil& inAirfoil, double inSpan, double inTipChord, double inRootChord, double inSweepAngle, double inWeight);  // Takes in inches, used for HT, and VT (ignore weight)
		// Wing(double inSpan, double inTipChord, double inRootChord, double inSweepAngle, ...);  // Constructor that takes in every variable


		// Copy constrcutor
		Wing(const Wing& other);

		// Useful Functions
		double calcReynolds(double velocity, double kinematicViscosity) const;
		double calcWettedArea() const;
		double calcWetRatio(double referenceArea) const;
		double calcDragCoeff(double AoA, double Reynolds, double Mach, double wetAreaRatio);       // AoA in radians
		double calcLiftCoeff(double AoA) const;							                           // AoA in radians
		double calcMcc(double AoA) const;                                                          // AoA in Radians



		// Mutators
		void setWeight(double inWeight);

		// Accessors
		double getArea() const;
		double getMAC() const;
		double getTaperRatio() const;
		double getAspectRatio() const;
		double getWeight() const;
		double getEllipticalEffic() const;
		double getSweepAngle() const;									// Returned in degrees
		double getSweepAngleRad() const;								// Returned in rad
		double getLeadingEdgeSweep() const;                             // Technically not an accessor.. but will treat it as one
		double getCL_Alpha() const;										// This returns the 3D CL_Alpha for CL = CL_Alpha * alpha + CL_Knott
		double getCL_Knott() const;										// This returns the 3D CL_Knott for CL = CL_Alpha * alpha + CL_Knott
		Airfoil* getAirfoil() const;




	private:
		// Minimum to take in:
		double tipChord;		// Stored in feet
		double rootChord;		// Stored in feet
		double span;			// Stored in feet
		double sweepAngle;		// Stored in rad (quarter chord)
		double ellipEfficiency; // Emperical equations are sketchy... Usually assuming .8
		Airfoil* airfoil;
		// double dihedralAngle;            // ignoring for now


		// Can calculate based on take ins:
		double area;                     // Stored in feet^2
		double weight;                   // Stored in lbms
		double aspectRatio;              // unitless
		double taperRatio;				 // unitless
		double MAC;						 // stored in feet
		LiftCoeff CL3D;



		static constexpr double pi = 3.141592653589;
		static constexpr double GAS_CONSTANT = 1716;




		double calcArea(double inTaperRatio) const;                     // Assumes Trapezodial Wing, Needs rootChord, span to be defined
		double calcMAC(double inTaperRatio) const;                      // Assumes Trapezodial Wing, Needs area, rootChord, taperRatio
		double calcAspectRatio(double inArea) const;
		double calcEllipEfficiency() const;                             // Needs aspectRatio, taperRatio, and sweepAngle to be defined


		void calcAndSetCL3D();											// Converts 2D Cl into 3D CL, and sets CL to this

		double calcMccZeroSweep(double AoA) const;						// AoA in radians, 0 deg Sweep Crest Critical Mach
		double calcSweptMExponent(double AoA) const;					// exponent "m" based on m vs CL graph

	};




}


#endif