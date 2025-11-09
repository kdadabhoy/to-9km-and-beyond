#ifndef WING_H
#define WING_H
// #include



namespace Airplane {
	class Wing {
	public:

		Wing();  // Default Constructor
		Wing(double inSpan, double inTipChord, double inRootChord, double inSweepAngle);  // Takes in inches, used for main wing
		Wing(double inSpan, double inTipChord, double inRootChord, double inSweepAngle, double inWeight);  // Takes in inches, used for HT, and VT (ignore weight)
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
		double getEllipticalEffic() const;           // Just calling this .8 for now
		double getC_L(const double AoA) const;                       // Just calling this 1 rn need to implement with airfoil stuff


	private:
		// Minimum to take in:
		double tipChord;      // Stored in feet
		double rootChord;     // Stored in feet
		double span;          // Stored in feet
		double sweepAngle;    // Stored in degrees
		// double dihedralAngle;            // ignoring for now


		// Can calculate based on take ins:
		double area;              // Stored in feet^2
		double weight;            // Stored in lbms
		double aspectRatio;       // unitless
		double taperRatio;        // unitless
		double MAC;               // stored in feet



		// Aero Lift Coeffs



		double calcArea(const double inTaperRatio) const;                     // Assumes Trapezodial Wing, Needs rootChord, span to be defined
		double calcMAC(const double inTaperRatio) const;                      // Assumes Trapezodial Wing, Needs area, rootChord, taperRatio
		double calcAspectRatio(const double inArea) const;


		double calcWeight() const;                                            // Need to figure out how to approx this


	};




}


#endif