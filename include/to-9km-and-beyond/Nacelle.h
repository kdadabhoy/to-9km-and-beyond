#ifndef NACELLE_H
#define NACELLE_H
// #include

namespace airplane {
	class Nacelle {
	public:
		Nacelle();
		Nacelle(double inWeight, double inLength, double inWettedArea);     // takes in lbm and inches
		Nacelle(const Nacelle& other);            // Copy Constructor
		Nacelle& operator=(const Nacelle& other); // Assignment Operator


		// Useful Functions:
		double calcReynolds(double velocity, double kinematicViscosity) const;
		double calcWetRatio(double referenceArea) const;
		double calcDragCoeff(double AoA_Rad, double Reynolds, double Mach, double wetAreaRatio) const;     // Takes in AoA in radian


		// Mutators:
		void setWeight(double inWeight);
		

		// Accessors:
		double getWeight() const;
		double getFormFactor() const;
		double getWettedArea() const;


	private:
		double weight;        // lbm
		double length;        // ft
		double wettedArea;    // Stored in ft^2

		// Could take these in as parameters... 
		// but they're fairly consistent across Nacelles (approxs anyway)
		double formFactor = 1.5;       // formFactor is ~1.0-2.0
	};




}


#endif