#ifndef TURBOFAN_H
#define TURBOFAN_H
// Base Class for CF34_3B1 (and possibly other turbofan engines)


namespace airplane {
	class TurboFan {

	public:
		TurboFan();
		TurboFan(double inWeight, double inStaticThrust);
		TurboFan(const TurboFan& other); // Copy Constructor

		// Accessors
		double getWeight() const;
		double getStaticThrust() const;



		// Mutators
		void setWeight(double inWeight);
		void setStaticThrust(double inThrust);


	private:

		double weight;				  // lbm
		double staticThrust;          // lbf, Sea-level static standard day thrust

		
		// Stuff for fuel consumption map per mach
		// Stuff for performance map per mach
	};

}


#endif