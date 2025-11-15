#include <iostream>
#include <vector>
using std::vector;
#include "to-9km-and-beyond/CF34_3B1.h"
#include "to-9km-and-beyond/TurboFan.h"
// Prob should add assignment operator at some point



namespace airplane {

	CF34_3B1::CF34_3B1() : TurboFan::TurboFan(1670, 19500)
	{
		// Assumes weight of CF34_3B1 is 1670 lbm
		// Assumes Standard Day Sea Level Static Thrust of CF34_3B1 is 19500 lb	
	}





	CF34_3B1::CF34_3B1(double inWeight, double inStaticThrust)
		: TurboFan::TurboFan(inWeight, inStaticThrust) 
	{
	}





	// Copy constrcutor
	CF34_3B1::CF34_3B1(const CF34_3B1& other)
		: TurboFan::TurboFan(other)
	{
		// And then do any specific CF34_3B1 copys
	}






	vector<double> CF34_3B1::getPowerCurveFunction(double height) {
		double interval = (5000 / 2); // Interval where we switch to next height power curve... instead of interpolating

		if (height < (5000 - interval)) {
			return h0PowerFunc;
		} else if (height < (10000 - interval)) {
			return h5kPowerFunc;
		} else if (height < (15000 - interval)) {
			return h10kPowerFunc;
		} else if (height < (20000 - interval)) {
			return h15kPowerFunc;
		} else if (height < (25000 - interval)) {
			return h20kPowerFunc;
		} else if (height < (30000 - interval)) {
			return h25kPowerFunc;
		} else if (height < (35000 - interval)) {
			return h30kPowerFunc;
		} else if (height < (40000 - interval)) {
			return h35kPowerFunc;
		} else if (height < (45000 - interval)) {
			return h40kPowerFunc;
		} else if (height < (50000 - interval)) {
			return h45kPowerFunc;
		} else if (height < (55000 - interval)) {
			return h50kPowerFunc;
		} else {
			return {};
		}
	}





}
