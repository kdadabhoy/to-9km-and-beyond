#ifndef WING_H
#define WING_H
#include "to-9km-and-beyond/Airfoil.h"
#include "to-9km-and-beyond/LiftCoeff.h"
// Prob should add assignment operator at some point


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
		double calcReynolds(const double velocity, const double kinematicViscosity) const;
		double calcWettedArea() const;

		// Accessors
		double getArea() const;
		double getMAC() const;
		double getTaperRatio() const;
		double getAspectRatio() const;
		double getWeight() const;
		double getAR() const;
		double getEllipticalEffic() const;								 // Just calling this .8 for now
		double getC_L_rad(const double AoA) const;                       // AoA passed in as radians




	private:
		// Minimum to take in:
		double tipChord;		// Stored in feet
		double rootChord;		// Stored in feet
		double span;			// Stored in feet
		double sweepAngle;		// Stored in degrees
		double ellipEfficiency; // Not sure if the equation we used is valid... but :)
		// double dihedralAngle;            // ignoring for now


		// Can calculate based on take ins:
		double area;              // Stored in feet^2
		double weight;            // Stored in lbms
		double aspectRatio;       // unitless
		double taperRatio;        // unitless
		double MAC;               // stored in feet
		static constexpr double pi = 3.141592653589;



		// Aero Lift Coeffs
		LiftCoeff CL3D;
		Airfoil* airfoil;

		double calcArea(const double inTaperRatio) const;                     // Assumes Trapezodial Wing, Needs rootChord, span to be defined
		double calcMAC(const double inTaperRatio) const;                      // Assumes Trapezodial Wing, Needs area, rootChord, taperRatio
		double calcAspectRatio(const double inArea) const;
		double calcEllipEfficiency(const double inAspectRatio) const;


		double calcWeight() const;                                            // Need to figure out how to approx this

		
		void calcCL3D();													  // Converts 2D Cl into 3D CL, and sets CL to this


	};




}


#endif