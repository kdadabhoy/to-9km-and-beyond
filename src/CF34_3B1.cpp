#include <iostream>
#include <vector>
using std::vector;
#include "to-9km-and-beyond/CF34_3B1.h"
#include "to-9km-and-beyond/TurboFan.h"
#include "to-9km-and-beyond/AtmosphereProperties.h"
using namespace atmosphere_properties;
#include "to-9km-and-beyond/kadenMath.h"
using kaden_math::evaluateFunction;
#include <cmath>
using std::sqrt;
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





	// y axis is Thrust/SSL Thrust
	// x axis is Mach #
	vector<double> CF34_3B1::getThrustCurveFunction(double height) const {
		double interval = (5000.0 / 2.0); // Interval where we switch to next height power curve... instead of interpolating

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











	double CF34_3B1::getThrust(double height, double velocity) const {
		AtmosphereProperties Cond(height);
		vector<double> thrustFunction = getThrustCurveFunction(height);
		double Mach = velocity / sqrt(1.4 * GAS_CONSTANT * Cond.getTemperature());
		return (evaluateFunction(thrustFunction, Mach) * getSLSThrust());          // lbf
	}










	// y axis is Fuel Loss (lbf_fuel / (lbf_thurst * hrs))
	// x axis is Mach #
	vector<double> CF34_3B1::getFuelLossFunction(double height) const {
		double interval = (5000 / 2); // Interval where we switch to next fuel loss function... instead of interpolating

		if (height < (5000 - interval)) {
			return h0FuelLossFunc;
		} else if (height < (10000 - interval)) {
			return h5kFuelLossFunc;
		} else if (height < (15000 - interval)) {
			return h10kFuelLossFunc;
		} else if (height < (20000 - interval)) {
			return h15kFuelLossFunc;
		} else if (height < (25000 - interval)) {
			return h20kFuelLossFunc;
		} else if (height < (30000 - interval)) {
			return h25kFuelLossFunc;
		} else if (height < (35000 - interval)) {
			return h30kFuelLossFunc;
		} else if (height < (40000 - interval)) {
			return h35kFuelLossFunc;
		} else if (height < (45000 - interval)) {
			return h40kFuelLossFunc;
		} else if (height < (50000 - interval)) {
			return h45kFuelLossFunc;
		} else if (height < (55000 - interval)) {
			return h50kFuelLossFunc;
		} else {
			return {};
		}
	}









	double CF34_3B1::calcFuelLoss(double timeInterval_Hours, double height, double velocity) const {
		AtmosphereProperties Cond(height);
		vector<double> fuelLossFunction = getFuelLossFunction(height);
		double Mach = velocity / sqrt(1.4 * GAS_CONSTANT * Cond.getTemperature());
		return (evaluateFunction(fuelLossFunction, Mach) * getThrust(height, velocity) * timeInterval_Hours); // lbf = lbm on Earth
	}









	double CF34_3B1::calcFuelLoss2(double timeInterval_Seconds, double height, double velocity) const {
		AtmosphereProperties Cond(height);
		vector<double> fuelLossFunction = getFuelLossFunction(height);
		double Mach = velocity / sqrt(1.4 * GAS_CONSTANT * Cond.getTemperature());
		return (evaluateFunction(fuelLossFunction, Mach) * getThrust(height, velocity) * timeInterval_Seconds * SECONDS_TO_HOURS); // lbf = lbm on Earth
	}


}
