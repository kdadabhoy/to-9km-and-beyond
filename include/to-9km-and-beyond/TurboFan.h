#ifndef TURBOFAN_H
#define TURBOFAN_H
// Base Class for CF34_3B1 (and possibly other turbofan engines)


namespace airplane {
	class TurboFan {

	public:
		TurboFan();
		TurboFan(double inWeight);
		TurboFan(const TurboFan& other); // Copy Constructor


		double getWeight() const;
		void setWeight(double inWeight);

	private:

		double weight;


		//double staticThrust = 19500;          // Sea-level static standard day thrust in lbs
		
		// Stuff for fuel consumption map per mach
		// Stuff for performance map per mach
	};

}


#endif