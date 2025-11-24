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

		// Thrust Functions (Power Curve)
		vector<double> getThrustCurveFunction(double height) const;  
		double getThrust(double height, double velocity) const;     // Returns current Thrust in lbf


		// Fuel Loss Functions
		vector<double> getFuelLossFunction(double height) const;   
		double calcFuelLoss(double timeInterval_Hours, double height, double velocity) const;   // returns how many lbf (=lbm on Earth) you lost
		double calcFuelLoss2(double timeInterval_Seconds, double height, double velocity) const;   // returns how many lbf (=lbm on Earth) you lost



	private:
		static constexpr double GAS_CONSTANT = 1716;
		static constexpr double SECONDS_TO_HOURS = (1.0 / 3600.0);

		// Have ability to get:
			//	weight
	

		// Probably put these in Base Class... but that's a later problem:
	
		// Power Functions from Performance Map Digitization
			// Notes:
			// (Thrust/SSL Thrust) = PowerFunc(Mach) for a set height
			// h30k -> h = 30,000 ft Power func
		const vector<double> h0PowerFunc   = { 0.723 , -0.962 , 1.000 };	   // 1.000 + -0.962x + 0.723x^2
		const vector<double> h5kPowerFunc  = { 0.563 , -0.775 , 0.906 };       // 0.906 + -0.775x + 0.563x^2
		const vector<double> h10kPowerFunc = { 0.469 , -0.672 , 0.804 };       // 0.804 + -0.672x + 0.469x^2
		const vector<double> h15kPowerFunc = { 0.433 , -0.580 , 0.700 };       // 0.700 + -0.580x + 0.433x^2
		const vector<double> h20kPowerFunc = { 0.310 , -0.508 , 0.594 };       // 0.594 + -0.508x + 0.310x^2
		const vector<double> h25kPowerFunc = { 0.277 , -0.425 , 0.498 };       // 0.498 + -0.425x + 0.277x^2
		const vector<double> h30kPowerFunc = { 0.250 , -0.396 , 0.423 };       // 0.423 + -0.396x + 0.250x^2

		// Not used in 9km project
		const vector<double> h35kPowerFunc = { 0.0482 , -0.0851 , 0.238 };     // 0.238 + -0.0851x + 0.0482x^2
		const vector<double> h40kPowerFunc = { 0.0482 , -0.0851 , 0.238 };     //
		const vector<double> h45kPowerFunc = { 0.0482 , -0.0851 , 0.238 };     //
		const vector<double> h50kPowerFunc = { 0.0482 , -0.0851 , 0.238 };     //




		// Fuel Loss Functions from Performance Map Digitization
			// Notes: 
			// *** PER HOUR ***
			// Fuel Loss (lbf_fuel / (lbf_thrust * hr)) = PowerFunc(Mach) for a set height
				// Fuel Loss lbm = (current fuel loss) * currentThrust * timeInterval
					// current fuel loss = evalute equation at a set height and your current Mach = f(mach, height)
			// h30k -> h = 30,000 ft Power func
		const vector<double> h0FuelLossFunc   = { 0.5069 , 0.3162 };	   // 0.5069x + 0.3162
		const vector<double> h5kFuelLossFunc  = { 0.5069 , 0.3162 };      // 
		const vector<double> h10kFuelLossFunc = { 0.5069 , 0.3162 };      //
		const vector<double> h15kFuelLossFunc = { 0.5069 , 0.3162 };      // 
		const vector<double> h20kFuelLossFunc = { 0.5069 , 0.3162 };      // 
		const vector<double> h25kFuelLossFunc = { 0.5069 , 0.3162 };      // 
		const vector<double> h30kFuelLossFunc = { 0.5069 , 0.3162 };      // 

		// Not used in 9km project
		const vector<double> h35kFuelLossFunc = { 0.5069 , 0.3162 };      // 
		const vector<double> h40kFuelLossFunc = { 0.5069 , 0.3162 };      //
		const vector<double> h45kFuelLossFunc = { 0.5069 , 0.3162 };      //
		const vector<double> h50kFuelLossFunc = { 0.5069 , 0.3162 };      //


	}; 


}


#endif