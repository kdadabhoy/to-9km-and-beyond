#ifndef FUSELAGE_H
#define FUSELAGE_H
// #include

namespace airplane {
	class Fuselage {
	public:
		Fuselage();
		Fuselage(double inWeight, double inLength, double inWettedArea);
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
		double getCL_Alpha() const;
		double getCL_Knott() const;

		// Mutators:
		void setWeight(double inWeight);


	private:
		double weight;
		double wettedArea;    // Stored in ft^2
		double length;


		// Could take these in as parameters... 
			// but they're fairly consistent across fuselages (approxs anyway)
		double formFactor = 1.2;       // formFactor is ~1.2-1.5
		double CL_alpha = .2;          // CL_alpha ~.1-.3
		double CL_knott = 0;           // CL_knott ~ 0... 
	
	};


	

}


#endif
