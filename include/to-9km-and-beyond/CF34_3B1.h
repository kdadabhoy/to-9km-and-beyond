#ifndef CF34_3B1_H
#define CF34_3B1_H
#include <vector>
using std::vector;
#include "to-9km-and-beyond/TurboFan.h"


namespace airplane {
	class CF34_3B1 : public TurboFan {
	public:
		CF34_3B1();												// Call this one, already has normal characteristics
		CF34_3B1(double inWeight, double inStaticThrust);       // If you ever wanted to have this enginge.. but different weight and staticThrust
		CF34_3B1(const CF34_3B1& other);						// Copy Constructor

		// Helpful Functions
		// Prob put this funciton in base class as well at some point...
		vector<double> getThrustCurveFunction(double height) const;
		double getThrust(double height, double velocity) const;



	private:
		static constexpr double GAS_CONSTANT = 1716;
		// Have ability to get:
			//	weight
			// 
		// Probably put these in Base Class... but that's a later problem:
		// Power Functions from Performance Map Digitization
			// (Thrust/SSL Thrust) = PowerFunc(Mach) for a set height
			// h0 -> h = 0 ft Power func
			// h30k -> h = 30,000 ft Power func
		const vector<double> h0PowerFunc =   { 0.723 , -0.962 , 1 };		 // 1 + -0.962x + 0.723x^2
		const vector<double> h5kPowerFunc =  { 0.563 , -0.775 , 0.906 };     // 0.906 + -0.775x + 0.563x^2
	
		const vector<double> h10kPowerFunc = { 0.469 , -0.672 , 0.804 };     // 0.804 + -0.672x + 0.469x^2
		const vector<double> h15kPowerFunc = { 0.433 , -0.580 , 0.700 };     // 0.7 + -0.58x + 0.433x^2
		const vector<double> h20kPowerFunc = { 0.310 , -0.508 , 0.594 };     // 0.594 + -0.508x + 0.31x^2
		const vector<double> h25kPowerFunc = { 0.277 , -0.425 , 0.498 };     // 0.498 + -0.425x + 0.277x^2
		const vector<double> h30kPowerFunc = { 0.277 , -0.425 , 0.498 };     // 

		// Not used in 9km project
		const vector<double> h35kPowerFunc = { 0.563 , -0.775 , 0.906 };     //
		const vector<double> h40kPowerFunc = { 0.563 , -0.775 , 0.906 };     //
		const vector<double> h45kPowerFunc = { 0.563 , -0.775 , 0.906 };     //
		const vector<double> h50kPowerFunc = { 0.563 , -0.775 , 0.906 };     //




	}; 


}


#endif