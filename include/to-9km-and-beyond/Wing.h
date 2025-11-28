#ifndef WING_H
#define WING_H
#include "to-9km-and-beyond/Airfoil.h"
#include "to-9km-and-beyond/LiftCoeff.h"
#include <vector>
using std::vector;
/*
	Notes:
		- Weight of main wing is calculated in Airplane Class because it requires knowledge of MTOW
		- setSpan() recomputes all the properties
		- Assumes Trapezodial Wing :)
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


	// Moment Approxs
		double calcRootMoment(double Lift) const;              // Returns lbf*ft
		double calcRootInertiaEstimate() const;                // Returns ft^4
		double calcLocalChord(double distanceFromRoot) const;  // Returns ft
		double calc_C_ForRootStress() const;                   // Returns ft


	// Maybe Useful Functions, Assume Trapezodial Wing
		double calcArea(double inSpan, double inRootChord, double inTaperRatio) const;                
		double calcMAC(double inRootChord, double inTaperRatio) const;                  
		double calcAspectRatio(double inSpan, double inArea) const;
		double calcEllipEfficiency(double inSweepAngleRad, double inAspectRatio, double inTaperRatio) const; 


	// Mutators
		void setWeight(double inWeight);
		void setSpan(double inSpan);


	// Accessors
		double getArea() const;                 // ft^2
		double getMAC() const;                  // ft
		double getRootChord() const;            // ft
		double getTaperRatio() const;    
		double getAspectRatio() const;  
		double getSpan() const;					// ft
		double getWeight() const;               // lbm
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


	// MccZeroSweep Constants (CL20 -> CL = .20)
		const vector<double> CL0 =  { 4.46 , -2.20 , 0.972 };		// 0.972 + -2.2x + 4.46x^2          
		const vector<double> CL20 = { 1.12 , -1.28 , 0.877 };       // 0.877 + -1.28x + 1.12x^2
		const vector<double> CL25 = { 2.23 , -1.51 , 0.876 };       // 0.876 + -1.51x + 2.23x^2
		const vector<double> CL30 = { 1.38 , -1.33 , 0.857 };       // 0.857 + -1.33x + 1.38x^2
		const vector<double> CL35 = { 3.13 , -1.68 , 0.864 };       // 0.864 + -1.68x + 3.13x^2 
		const vector<double> CL40 = { 1.70 , -1.31 , 0.833 };       // 0.833 + -1.31x + 1.70x^2
		const vector<double> CL45 = { 1.96 , -1.34 , 0.824 };       // 0.824 + -1.34x + 1.96x^2
		const vector<double> CL50 = { 2.90 , -1.53 , 0.824 };       // 0.824 + -1.53x + 2.90x^2
		const vector<double> CL55 = { 2.68 , -1.46 , 0.810 };       // 0.810 + -1.46x + 2.68x^2
		const vector<double> CL60 = { 2.32 , -1.34 , 0.792 };       // 0.792 + -1.34x + 2.32x^2



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