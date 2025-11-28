#ifndef TURBOFAN_H
#define TURBOFAN_H
// Base Class for CF34_3B1 (and possibly other turbofan engines)
// Worry about virtual keyword later, since I will always us member functions with derived object

// For all intensive MAE 158 Project Purposes - this file is ignorable

namespace airplane {
	class TurboFan {

	public:
		TurboFan();
		TurboFan(double inWeight, double inSSLThrust);    // lbm and lbf
		TurboFan(const TurboFan& other);				  // Copy Constructor


		// Accessors
		double getWeight() const;
		double getSLSThrust() const;
		double getSpecificFuelConsumption() const;


		// Mutators
		void setWeight(double inWeight);
		void setSLSThrust(double inThrust);
		void setSpecificFuelConsumption(double inFrac);

		// Useful Calc
		//double getPowerCurveFunction(double height);


	private:
		double weight;						// lbm
		double SLSThrust;				    // lbf, Sea-level static standard day thrust
		double specificFuelConsumption;		// should change based off Mach (Performance map)


		
		// Stuff for fuel consumption map per mach
		// Stuff for performance map per mach
	};

}


#endif