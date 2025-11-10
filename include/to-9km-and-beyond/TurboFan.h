#ifndef TURBOFAN_H
#define TURBOFAN_H
// Base Class for CF34_3B1 (and possibly other turbofan engines)
// Worry about virtual keyword later, since I will always us member functions with derived object

namespace airplane {
	class TurboFan {

	public:
		TurboFan();
		TurboFan(double inWeight, double inSSLThrust);
		TurboFan(const TurboFan& other); // Copy Constructor

		// Accessors
		double getWeight() const;
		double getSSLThrust() const;
		double getThrustPerSSLThrust() const;
		double getSpecificFuelConsumption() const;


		// Mutators
		void setWeight(double inWeight);
		void setSSLThrust(double inThrust);
		void setThrustPerSSLThrust(double inFrac);
		void setSpecificFuelConsumption(double inFrac);

		// Useful Calc
		double getThrust() const;


	private:
		double weight;						// lbm
		double SSLThrust;				    // lbf, Sea-level static standard day thrust
		double specificFuelConsumption;		// should change based off Mach (Performance map)
		double thrustPerSSLThrust;          // should change based off Mach (Performance map), it's a fraction < 1 typically


		
		// Stuff for fuel consumption map per mach
		// Stuff for performance map per mach
	};

}


#endif