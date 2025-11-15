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
		vector<double> getPowerCurveFunction(double height);




	private:
		// Have ability to get:
			//	weight
			// 
		// Probably put these in Base Class... but that's a later problem:
		// Power Functions from Performance Map Digitization
			// (Thrust/SSL Thrust) = PowerFunc(Mach) for a set height
			// h0 -> h = 0 ft Power func
			// h30k -> h = 30,000 ft Power func
		const vector<double> h0PowerFunc = { 0.723 ,-0.962 , 1 };
		const vector<double> h5kPowerFunc = { 0.563 ,-0.775 , 0.906 };
	
		const vector<double> h10kPowerFunc = { 0.563 ,-0.775 , 0.906 };
		const vector<double> h15kPowerFunc = { 0.563 ,-0.775 , 0.906 };
		const vector<double> h20kPowerFunc = { 0.563 ,-0.775 , 0.906 };
		const vector<double> h25kPowerFunc = { 0.563 ,-0.775 , 0.906 };
		const vector<double> h30kPowerFunc = { 0.563 ,-0.775 , 0.906 };
		const vector<double> h35kPowerFunc = { 0.563 ,-0.775 , 0.906 };
		const vector<double> h40kPowerFunc = { 0.563 ,-0.775 , 0.906 };
		const vector<double> h45kPowerFunc = { 0.563 ,-0.775 , 0.906 };
		const vector<double> h50kPowerFunc = { 0.563 ,-0.775 , 0.906 };

	}; 


}


#endif