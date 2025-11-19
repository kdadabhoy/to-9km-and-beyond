#ifndef WING_H
#define WING_H
#include "to-9km-and-beyond/Airfoil.h"
#include "to-9km-and-beyond/LiftCoeff.h"
/*
	Notes:
		- Weight of main wing is calculated in Airplane Class because it requires knowledge of MTOW
		- Probably should add the big 3 to make it a dynamic class
		- setSpan() recomputes all the properties
		- calcMccZeroSweep() function still needs digitization values (incomplete rn)
*/


namespace airplane {
	class Wing {
	public:
	// Constructors
		Wing(); 
		Wing(Airfoil& inAirfoil, double inSpanInch, double inTipChordInch, double inRootChordInch, double inSweepAngleDeg);  
		Wing(Airfoil& inAirfoil, double inSpanInch, double inTipChordInch, double inRootChordInch, double inSweepAngleDeg, double inWeight);
		Wing(const Wing& other); // Copy constrcutor


	// Useful Functions
		double calcReynolds(double velocity, double kinematicViscosity) const;
		double calcWettedArea() const;
		double calcWetRatio(double referenceArea) const;
		double calcDragCoeff(double AoA_rad, double Reynolds, double Mach, double wetAreaRatio) const; 
		double calcLiftCoeff(double AoA_rad) const;													 
		double calcMcc(double AoA_rad) const; 


	// Maybe Useful Functions, Assume Trapezodial Wing
		double calcArea(double inSpan, double inRootChord, double inTaperRatio) const;                
		double calcMAC(double inRootChord, double inTaperRatio) const;                  
		double calcAspectRatio(double inSpan, double inArea) const;
		double calcEllipEfficiency(double inSweepAngleRad, double inAspectRatio, double inTaperRatio) const; 


	// Mutators
		void setWeight(double inWeight);
		void setSpan(double inSpan);



	// Accessors
		double getArea() const;
		double getMAC() const;
		double getTaperRatio() const;
		double getAspectRatio() const;
		double getWeight() const;
		double getEllipticalEffic() const;
		double getSweepAngle() const;			// Returned in degrees
		double getSweepAngleRad() const;		// Returned in rad
		double getLeadingEdgeSweep() const;     // Technically not an accessor.. but will treat it as one
		double getCL_Alpha() const;			    // This returns the 3D CL_Alpha for CL = CL_Alpha * alpha + CL_Knott
		double getCL_Knott() const;			    // This returns the 3D CL_Knott for CL = CL_Alpha * alpha + CL_Knott
		Airfoil* getAirfoil() const;


	private:
	// Minimum to take in:
		double tipChord;						// Stored in ft
		double rootChord;						// Stored in ft
		double span;							// Stored in ft
		double sweepAngle;						// Stored in rad (quarter chord)
		double ellipEfficiency;					// 0.8 if swept (emperical formulas suck), emperical formula if unswept
		Airfoil* airfoil;


	// Can calculate based on take ins:
		double area;							// Stored in ft^2
		double weight;							// Stored in lbms
		double aspectRatio;						// unitless
		double taperRatio;				        // unitless
		double MAC;						        // stored in ft
		LiftCoeff CL3D;


	// Constants
		static constexpr double pi = 3.141592653589;
		static constexpr double GAS_CONSTANT = 1716;


	// calcAndSet Functions, Assumes Trapezodial Wing
		void calcAndSetAllProperties();		    // Needs span, rootChord, taperRatio, and airfoil to be defined
		void calcAndSetArea();				    // Needs span, rootChord, and taperRatio to be defined; Calcs S
		void calcAndSetMAC();				    // Needs rootChord and taperRatio to be defined; Cals MAC
		void calcAndSetAspectRatio();		    // Needs span and area to be defined; Calcs AR
		void calcAndSetEllipEfficiency();       // Needs sweepAngle, aspectRatio, and taperRatio to be defined; Calcs e emperically 
		void calcAndSetCL3D();				    // Needs sweepAngle, aspectRatio, and airfoil to be defined; Converts 2D Cl into 3D CL,


	// Sweep Functions
		double calcMccZeroSweep(double AoA_rad) const;		// 0 deg Sweep Crest Critical Mach
		double calcSweptMExponent(double AoA_rad) const;	// Exponent "m" based on m vs CL graph
	};
}


#endif