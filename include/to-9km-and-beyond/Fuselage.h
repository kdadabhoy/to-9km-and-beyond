#ifndef FUSELAGE_H
#define FUSELAGE_H
// #include

namespace airplane {
	class Fuselage {
	public:
		Fuselage();
		Fuselage(double inWeight);
		Fuselage(const Fuselage& other);				// Copy Constructor
		Fuselage& operator=(const Fuselage& other);		// Assignment Operator

		// Helpful Functions
		double calcReynolds(double velocity, double kinematicViscosity) const;
		double calcWetRatio(double referenceArea) const;
		double calcLiftCoeff(double AoA) const;														 // Takes in AoA in radian
		double calcDragCoeff(double AoA, double Reynolds, double Mach, double wetAreaRatio) const;     // Takes in AoA in radian


		//Accessors:
		double getWeight() const;
		double getFormFactor() const;
		double getWettedArea() const;

		// Mutators:
		void setWeight(double inWeight);


	private:
		double weight;

		//Probably take these in as Parameters in constructor later.
		double formFactor = 1.2;       // formFactor is ~1.2-1.5
		double CL_alpha = .2;          // CL_alpha ~.1-.3
		double CL_knott = 0;           // CL_knott ~ 0... 
		double wettedArea = 811.55;    // Stored in ft^2
		double length = 100.00;           // Stored in ft^2 need to verify this on CAD
	};


	

}


#endif
