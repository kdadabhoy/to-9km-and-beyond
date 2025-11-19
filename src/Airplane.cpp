#include <iostream>
#include <cassert>
#include <cmath>
#include <string>
using std::string;
#include "to-9km-and-beyond/Airplane.h"
#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/TurboFan.h"
#include "to-9km-and-beyond/CF34_3B1.h"
#include "to-9km-and-beyond/Nacelle.h"
#include "to-9km-and-beyond/AtmosphereProperties.h"
#include "to-9km-and-beyond/kadenMath.h"
using kaden_math::saveVectorsToCSV;
using kaden_math::evaluateFunction;
using kaden_math::maxDistBetweenCurves2;
using kaden_math::linearInterpolate;
using std::floor;

using namespace std;
using namespace atmosphere_properties;
// Assumes we are passing in the full span of the Wing and HT (in other words they are symmetrical)
	// In other, other words, the Wing objects represent the full main wing and full HT 
// Prob should add assignment operator at some point


namespace airplane {

	Airplane::Airplane() {
		mainWing = nullptr;
		HT = nullptr;
		VT = nullptr;
		engine = nullptr;
		nacelle = nullptr;
		fuselage = nullptr;
		fuelWeight = 0; 
		payLoadWeight = 0; 
		referenceArea = 0;
		totalWeight = 0;
		CL.setCL_Alpha(0);
		CL.setCL_Knott(0);
	}








	// don't do const Class& object, because I might want to modify the object in this class...
	// If I decide none of the objects get modified in this class, then add const here and the pointer won't
	// allow modificaiton to the passed object
	// Would also have to change all member vairbales to const Class*
	Airplane::Airplane(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage, double inFuelWeight, double inPayLoadWeight){
		mainWing = &inWing;
		HT = &inHT;
		VT = &inVT;
		engine = &inEngine;
		nacelle = &inNacelle;
		fuselage = &inFuselage;
		fuelWeight = inFuelWeight;
		payLoadWeight = inPayLoadWeight; 
		assert(mainWing != nullptr);
		referenceArea = mainWing->getArea();
		calcAndSetLiftCoeff();
		calcAndSetTotalWeight();
	}





	void Airplane::calcAndSetTotalWeight() {
		double total = 0.0;

		if (mainWing) {
			total += mainWing->getWeight();
		}

		if (HT) {
			total += (HT->getWeight() * 2);
		}

		if (VT) {
			total += VT->getWeight();
		}

		if (engine) {
			total += (engine->getWeight() * numEngines);
		}

		if (nacelle) {
			total += (nacelle->getWeight() * 2);
		}

		if (fuselage) {
			total += fuselage->getWeight();
		}

		totalWeight = total;
		return;
	}



	// Can make this safer by mimicing totalWeight function's if statements
	void Airplane::calcAndSetLiftCoeff() {
		double alphaTerm = 0;
		double knottTerm = 0;

		// Assume VT contributes negligble lift.
		alphaTerm = mainWing->getCL_Alpha() + HT->getCL_Alpha() + fuselage->getCL_Alpha();
		knottTerm = mainWing->getCL_Knott() + HT->getCL_Knott() + fuselage->getCL_Knott();

		CL.setCL_Alpha(alphaTerm);
		CL.setCL_Knott(knottTerm);
		return;
	}





// Drag Functions
	double Airplane::calcDragCoeff(double AoA, double velocity, double Mach, double kinematicViscosity) const {
		double totalDrag = 0;

		totalDrag += mainWing->calcDragCoeff(AoA, mainWing->calcReynolds(velocity, kinematicViscosity), Mach, mainWing->calcWetRatio(referenceArea));
		totalDrag += HT->calcDragCoeff(AoA, HT->calcReynolds(velocity, kinematicViscosity), Mach, HT->calcWetRatio(referenceArea));
		totalDrag += fuselage->calcDragCoeff(AoA, fuselage->calcReynolds(velocity, kinematicViscosity), Mach, fuselage->calcWetRatio(referenceArea));
		// Might need to add drag for VT, but just the parasite drag (inducded drag acts horizontally?)

		return totalDrag;
	}





	double Airplane::calcDrag(double AoA, double velocity, double Mach, double kinematicViscosity, double density) const {
		double totalDrag = 0;

		totalDrag += mainWing->calcDragCoeff(AoA, mainWing->calcReynolds(velocity, kinematicViscosity), Mach, mainWing->calcWetRatio(referenceArea));
		totalDrag += HT->calcDragCoeff(AoA, HT->calcReynolds(velocity, kinematicViscosity), Mach, HT->calcWetRatio(referenceArea));
		totalDrag += fuselage->calcDragCoeff(AoA, fuselage->calcReynolds(velocity, kinematicViscosity), Mach, fuselage->calcWetRatio(referenceArea));
		// Might need to add drag for VT, but just the parasite drag (inducded drag acts horizontally?)

		totalDrag = 0.5 * density * velocity * velocity * referenceArea * totalDrag;

		return totalDrag;
	}






// Lift Functions:
	double Airplane::calcLiftCoeff(double AoA) const {
		return CL.calcLiftCoefficient(AoA);
	}

	






	double Airplane::calcLift(double AoA, double velocity, double density) const {
		return  0.5 * density * velocity * velocity * referenceArea * CL.calcLiftCoefficient(AoA);
	}








// Useful Functions:

	double Airplane::calcMach(double velocity, double temp) const {
		return velocity / sqrt(1.4 * GAS_CONSTANT * temp);
	}
















	// Calc Alpha needed for Steady Level Climb at a Gamma
		// L = W*cos(gamma)
	double Airplane::calcSteadyClimbAoA(double gamma, double velocity, double density) const {
		assert(velocity > 0.00 && density > 0.00 && CL.getCL_Alpha() > 0.00);

		double CL_needed = 0;
		double gamma_rad = gamma * pi / 180;
		double AoA_needed = 0;

		CL_needed = (totalWeight * cos(gamma_rad)) / (0.5 * velocity * velocity * density * referenceArea);
		CL_needed = CL_needed - CL.getCL_Knott();
		AoA_needed = CL_needed / CL.getCL_Alpha();

		return AoA_needed;
	}



	// Small Angle assumption (L = W*cos(0) = W)
	double Airplane::calcSteadyClimbAoAApprox(double velocity, double density) const {
		assert(velocity > 0.00 && density > 0.00 && CL.getCL_Alpha() > 0.00);

		double CL_needed = 0;
		double AoA_needed = 0;

		CL_needed = (totalWeight) / (0.5 * velocity * velocity * density * referenceArea);
		CL_needed = CL_needed - CL.getCL_Knott();
		AoA_needed = CL_needed / CL.getCL_Alpha();

		return AoA_needed;
	}











// Power Curve Functions


	vector<double> Airplane::calcPowerCurveMachData() const {
		vector<double> data;

		double dx = (X_MAX - X_MIN) / STEPS;   // 1000 steps between mach~=0 and mach~=1
		for (int i = 0; i < STEPS; i++) {
			double Mach = (i * dx) + X_MIN;
			data.push_back(Mach);
		}
		return data;
	}







	vector<double> Airplane::calcPowerCurveVelocityData(double height) const {
		vector<double> data;
		AtmosphereProperties Cond(height);
		double temp = Cond.getTemperature();

		double dx = (X_MAX - X_MIN) / STEPS;   // 1000 steps between mach~=0 and mach~=1
		for (int i = 0; i < STEPS; i++) {
			double Mach = (i * dx) + X_MIN;
			double velocity = Mach * sqrt(1.4 * GAS_CONSTANT * temp);
			data.push_back(velocity);
		}
		return data;
	}







	vector<double> Airplane::calcPowerAvailableData(double height) const {
		vector<double> xdata = calcPowerCurveVelocityData(height);
		vector<double> ydata;
		vector<double> thrustCurve = engine->getThrustCurveFunction(height);
		AtmosphereProperties Cond(height);
		double tempature = Cond.getTemperature();
		double SLSThrust = engine->getSLSThrust();

		for (int i = 0; i < xdata.size(); i++) {
			double xtemp = xdata[i] / (sqrt(1.4 * GAS_CONSTANT * tempature));
			double ytemp = evaluateFunction(thrustCurve, xtemp);
			ytemp = ytemp * numEngines * SLSThrust * xdata[i];
			ydata.push_back(ytemp);
		}
		return ydata;
	}







	void Airplane::calcAndSetMaxExcessPower() {
		vector<double> result = maxDistBetweenCurves2(powerCurveVelocityData, powerAvailableData, powerRequiredData);
		if (result.empty()) {
			velocityMaxExcessPower = -1;
			maxExcessPower = 0;
		} else {
			velocityMaxExcessPower = result[0];
			maxExcessPower = result[1];
		}

		return;
	}







	// double check that the interpolation is working accurately sometime
		// It's close.. but not sure it's 100% accurate
	double Airplane::calcExcessPower(double velocity) const {
		double x_min = powerCurveVelocityData[0];
		double x_max = powerCurveVelocityData[powerCurveVelocityData.size() - 1];
		double dx = (x_max - x_min) / STEPS;   // Power Curve velocities have known spacing
		double point = (velocity - x_min) / dx;
		int lowerIndex = static_cast<int>(floor(point));
		int upperIndex = lowerIndex + 1;

		// Maybe possible to do without this interpolation method bc evenly spaced...
		double powerAvail = linearInterpolate(velocity, powerCurveVelocityData[lowerIndex], powerAvailableData[lowerIndex], powerCurveVelocityData[upperIndex], powerAvailableData[upperIndex]);
		double powerReq = linearInterpolate(velocity, powerCurveVelocityData[lowerIndex], powerRequiredData[lowerIndex], powerCurveVelocityData[upperIndex], powerRequiredData[upperIndex]);

		return (powerAvail - powerReq);
	}












// 100% Accurate (gamma accounted for):

	void Airplane::calcAndSetPowerCurveData(double gamma, double height) {
		if (powerCurveVelocityData.empty()) {
			powerCurveVelocityData = calcPowerCurveVelocityData(height);
		}

		powerRequiredData = calcPowerRequiredData(gamma, height);
		powerAvailableData = calcPowerAvailableData(height);
		calcAndSetMaxExcessPower();

		return;
	}





	vector<double> Airplane::calcPowerRequiredData(double gamma, double height) const {
		AtmosphereProperties Cond(height);
		vector<double> data;
		double density = Cond.getDensity();
		double kineVisc = Cond.getKinematicVisc();
		double temp = Cond.getTemperature();

		double dx = (X_MAX - X_MIN) / STEPS;     // 1000 steps between mach~=0 and mach~=1
		for (int i = 0; i < STEPS; i++) {
			double Mach = (i * dx) + X_MIN;
			double velocity = Mach * sqrt(1.4 * GAS_CONSTANT * temp);
			double AoA = calcSteadyClimbAoA(gamma, velocity, density);
			data.push_back(calcDrag(AoA, velocity, Mach, kineVisc, density) * velocity);
		}
		return data;
	}











	// Slightly inefficent bc the calling functions create their own AtmosProp Object
	void Airplane::getPowerCurveCSV(double gamma, double height, string fileName) const {
		vector<double> xdata = calcPowerCurveVelocityData(height);
		vector<double> y1data = calcPowerRequiredData(gamma, height);
		vector<double> y2data = calcPowerAvailableData(height);
		saveVectorsToCSV(xdata, y1data, y2data, fileName);
		return;
	}










// Small Angle Approx (cos(gamma) ~= 1)

	void Airplane::calcAndSetPowerCurveData(double height) {
		if (powerCurveVelocityData.empty()) {
			powerCurveVelocityData = calcPowerCurveVelocityData(height);
		}

		powerRequiredData = calcPowerRequiredData(height);
		powerAvailableData = calcPowerAvailableData(height);
		calcAndSetMaxExcessPower();

		return;
	}








	vector<double> Airplane::calcPowerRequiredData(double height) const {
		AtmosphereProperties Cond(height);
		vector<double> data;
		double density = Cond.getDensity();
		double kineVisc = Cond.getKinematicVisc();
		double temp = Cond.getTemperature();

		double dx = (X_MAX - X_MIN) / STEPS;     // 1000 steps between mach~=0 and mach~=1
		for (int i = 0; i < STEPS; i++) {
			double Mach = (i * dx) + X_MIN;
			double velocity = Mach * sqrt(1.4 * GAS_CONSTANT * temp);
			double AoA = calcSteadyClimbAoAApprox(velocity, density);
			data.push_back(calcDrag(AoA, velocity, Mach, kineVisc, density) * velocity);
		}
		return data;
	}






	// Slightly inefficent bc the calling functions create their own AtmosProp Object
	void Airplane::getPowerCurveCSV(double height, string fileName) const {
		vector<double> xdata = calcPowerCurveVelocityData(height);
		vector<double> y1data = calcPowerRequiredData(height);
		vector<double> y2data = calcPowerAvailableData(height);
		saveVectorsToCSV(xdata, y1data, y2data, fileName);
		return;
	}





















// Climb Functions
// 
		// (ROC)_max = (Excess Power)_max / Weight
			// Excess Power = Power Avaiable - Power Required
				// Power Required = Total Drag * velocity

		// Time to Climb = integral from h1 to h2 (dh / RoC)
			// ROC * change in time = dh
		// RoC = (P_avail - P_req) / weight
		// dh = (RoC)dt






	// Test this

	// *** Need to account for weight loss ***
	// Uses small angle approx for gamma
	double Airplane::calcBestClimbTime(double startHeight, double startVelocity, double endHeight) {
		// Implementing an accelerate/declerate in a straight line then climb at maxExcessPower
		// Might need to implement AoA < 15 deg or smt checks
		double totalTime = 0;
		double height = startHeight;
		double velocity = startVelocity;
		double VELOCITY_ERROR = .5;						// Can Change if too inefficient
		double TIME_STEP = .1;							// Can Change if too inefficient


		while (height <= endHeight) {
			calcAndSetPowerCurveData(height);
			calcAndSetMaxExcessPower();

			if (fabs(velocity - velocityMaxExcessPower) > VELOCITY_ERROR) {
				totalTime += calcSteadyLevelAccelerationTime(velocity, velocityMaxExcessPower, height);
				velocity = velocityMaxExcessPower;
			} else {
				height += (maxExcessPower / totalWeight) * TIME_STEP; // small angle approx method
				totalTime += TIME_STEP;

				/*
					// If you want to see how gamma changes during flight
					double gamma = asin(maxExcessPower / (totalWeight * velocityMaxExcessPower));
					cout << "gamma (deg): " << gamma * 180 / 3.1415 << endl;
				*/
			}
		}
		return totalTime;
	}













	// *** Need to account for weight loss ***
	// Only evalutes Power Curves when height changes by 500
	double Airplane::calcBestClimbTimeApprox(double startHeight, double startVelocity, double endHeight) {
		// Implementing an accelerate/declerate in a straight line then climb at maxExcessPower
		double totalTime = 0;
		double height = startHeight;
		double velocity = startVelocity;
		double VELOCITY_ERROR = .5;						// Can Change if too inefficient
		double TIME_STEP = .1;                      // Can Change if too inefficient


		double oldHeight = startHeight;
		double HEIGHT_STEP = 500;

		calcAndSetPowerCurveData(height);
		calcAndSetMaxExcessPower();
		while (height <= endHeight) {

			if (height - oldHeight >= HEIGHT_STEP) {
				calcAndSetPowerCurveData(height);	// Small angle approx, gets me my power curve
				calcAndSetMaxExcessPower();			// Gets me my max Excess Power and what velocity it is at
				oldHeight = height;
			}

			if (fabs(velocity - velocityMaxExcessPower) > VELOCITY_ERROR) {
				totalTime += calcSteadyLevelAccelerationTime(velocity, velocityMaxExcessPower, height);
				velocity = velocityMaxExcessPower;
			} else {
				height += (maxExcessPower / totalWeight) * TIME_STEP;
				totalTime += TIME_STEP;
			}
		}
		return totalTime;
	}




















// Steady Level Flight (L=W) Functions

	double Airplane::calcSteadyLevelAoA(double velocity, double density) const {
		assert(velocity > 0.00 && density > 0.00 && CL.getCL_Alpha() > 0.00);

		double CL_needed = 0;
		double AoA_needed = 0;

		CL_needed = (totalWeight) / (0.5 * velocity * velocity * density * referenceArea);
		CL_needed = CL_needed - CL.getCL_Knott();
		AoA_needed = CL_needed / CL.getCL_Alpha();

		return AoA_needed;
	}











	// *** Need to account for weight loss ***
	// Constant height Max Horizontal Acceleration
		// In a straight line F = ma, a = F/m
		// Max Acceleration = (ThrustAvail - Drag) / weight
		// change in v = acceleration * dt
		// change in v = (acceleration - v0) / time
	double Airplane::calcSteadyLevelAccelerationTime(double startVelocity, double finalVelocity, double height) {
		double totalTime = 0;
		double velocity = startVelocity;

		AtmosphereProperties Cond(height);
		double temp = Cond.getTemperature();
		double kineVisc = Cond.getKinematicVisc();
		double density = Cond.getDensity();
		double Mach, thrust, AoA, drag, maxAcceleration, engineFactor;

		double TIME_STEP = 0.05;        // Can Change if too inefficient
		double VELOCITY_ERROR = .5;		  // Can Change if too inefficient
		double velDifference = finalVelocity - startVelocity;

		while (fabs(velDifference) > VELOCITY_ERROR) {
			Mach = calcMach(velocity, temp);
			AoA = calcSteadyLevelAoA(velocity, density);
			drag = calcDrag(AoA, velocity, Mach, kineVisc, density);

			if (velDifference > 0) {
				// Accelerate case
				engineFactor = 1;
			} else {
				// Decelerate
				engineFactor = .20;	 // Thrust to 20% max thrust (brake using drag)... could use some PID thing instead.. but this is good enough
									 // Set this to 0% for best possible time... but real life is ~20% of thrust being minimum
			}

			thrust = numEngines * engine->getThrust(height, velocity) * engineFactor;
			maxAcceleration = ((thrust - drag) * GRAVITY) / totalWeight;                // Don't forget gravity... bc imperial!
			velocity = (maxAcceleration * TIME_STEP) + velocity;
			totalTime = totalTime + TIME_STEP;
			velDifference = finalVelocity - velocity;

			assert(fabs(AoA) < (20 * 3.1415) / 180);                                    // Just a precaution to make sure AoA never goes above 20 deg (optimisitic)
		}

		return totalTime;
	}










// Takeoff Functions

	// *** Need to account for weight loss ***

	// This function assumes we start climbing at V2 = 1.2V_stall = 1.2 * sqrt((2*W) / (rho*CL_max*S))
	// And we climb until we reach 500 ft.. which is on the low end of FAA regs
		// I have doubts that this is the best way to takeoff for a time-to-climb... 
		// but for realism and the purpose of integrating lec content into this program
		// we will take this approach

	// V2 = 1.2V_stall = 1.2 * sqrt((2*W) / (rho*CL_max*S))
	// Rolling Resistance = mu(W-(q*S*C_L))
		// mu ~= .02
	// a  = F/m
		// Force = Thrust - Drag - Rolling Resistance
		// F = T - qSC_D - mu(W-(q*S*C_L))
		// m = W/g
	double Airplane::calcTakeoffTime(double height, double endHeight) {
		double totalTime = 0;
		double velocity = 0;  // Start from rest 

		//double CL_Stall = mainWing->get...   // Need to code a function that gets this from my airfoil and converts that to 3D
		double AoA_Stall = 15 * pi / 180;      // Just using a realistic, arbirtary AoA rn
		double CL_Stall = calcLiftCoeff(AoA_Stall);

		AtmosphereProperties Cond(height);
		double density = Cond.getDensity();
		double temp = Cond.getTemperature();
		double kineVisc = Cond.getKinematicVisc();

		double V2 = 1.2 * sqrt((2 * totalWeight) / (density * CL_Stall * mainWing->getArea()));

		double Mach, thrust, AoA, drag, lift, rollingFriction, maxAcceleration;
		double ROLLING_FRIC_COEFF = .02; // A realistic approx
		double TIME_STEP = 0.05;        // Can Change if too inefficient

		while (velocity < V2) {
			Mach = calcMach(velocity, temp);
			AoA = 2 * pi / 180;				 // An Assumption, it is very close to 0... while on runway
			drag = calcDrag(AoA, velocity, Mach, kineVisc, density);
			lift = calcLift(AoA, velocity, density);
			rollingFriction = ROLLING_FRIC_COEFF * (totalWeight - lift);

			thrust = numEngines * engine->getThrust(height, velocity);
			maxAcceleration = ((thrust - drag - rollingFriction) * GRAVITY) / totalWeight;                // Don't forget gravity... bc imperial!
			velocity = (maxAcceleration * TIME_STEP) + velocity;
			totalTime = totalTime + TIME_STEP;
		}

		assert(velocity > V2); // Precaution to make sure we accelerated enough

	
		while (height < endHeight) {
			calcAndSetPowerCurveData(height);
			double excessPower = calcExcessPower(velocity);
			height += (excessPower / totalWeight) * TIME_STEP; // small angle approx method
			totalTime += TIME_STEP;
			// Just a precaution to make sure AoA never goes above 15 deg (optimisitic)
			AoA = calcSteadyClimbAoAApprox(velocity, density);
			assert(fabs(AoA) < (15 * 3.1415) / 180); 

		}
		return totalTime;
	}











	// A modified calcTakeOffTime (same logic)... but "returns" (modifies the passed by reference variables:
	// endVelocity and totalTime and endHeight... which is often needed
	// Note: a common FAA endheight is around 500 ft... the best endHeight for climb is 0 ft (with the way the program is written)
		// Technically 0 ft shouldn't be the best... unless you were somehow able to get rid of your wheels...
		// But with the assumption you are able to steady level accelerate at 0 ft... and ignoring ground effect... 0 ft is the best
	void Airplane::calcTakeoffPropertites(double height, double& endHeight, double& totalTime, double& velocity) {
		calcEndRunwayAirplaneProperties(height, totalTime, velocity);
		AtmosphereProperties Cond(height);
		double density = Cond.getDensity();
		double TIME_STEP = 0.05;                 // Can Change if too inefficient
		double AoA = 2 * pi / 180;				 // An Assumption, it is very close to 0... while on runway
		

		while (height < endHeight) {
			calcAndSetPowerCurveData(height);
			double excessPower = calcExcessPower(velocity);
			height += (excessPower / totalWeight) * TIME_STEP; // small angle approx method
			totalTime += TIME_STEP;
			// Just a precaution to make sure AoA never goes above 15 deg (optimisitic)
			AoA = calcSteadyClimbAoAApprox(velocity, density);
			assert(fabs(AoA) < (15 * 3.1415) / 180);

		}
		endHeight = height;
	}













	void Airplane::calcEndRunwayAirplaneProperties(double height, double& totalTime, double& velocity) const {

		//double CL_Stall = mainWing->get...   // Need to code a function that gets this from my airfoil and converts that to 3D
		double AoA_Stall = 15 * pi / 180;      // Just using a realistic, arbirtary AoA rn
		double CL_Stall = calcLiftCoeff(AoA_Stall);

		AtmosphereProperties Cond(height);
		double density = Cond.getDensity();
		double temp = Cond.getTemperature();
		double kineVisc = Cond.getKinematicVisc();

		double V2 = 1.2 * sqrt((2 * totalWeight) / (density * CL_Stall * mainWing->getArea()));

		double Mach, thrust, AoA, drag, lift, rollingFriction, maxAcceleration;
		double ROLLING_FRIC_COEFF = .02; // A realistic approx
		double TIME_STEP = 0.05;        // Can Change if too inefficient

		while (velocity < V2) {
			Mach = calcMach(velocity, temp);
			AoA = 2 * pi / 180;				 // An Assumption, it is very close to 0... while on runway
			drag = calcDrag(AoA, velocity, Mach, kineVisc, density);
			lift = calcLift(AoA, velocity, density);
			rollingFriction = ROLLING_FRIC_COEFF * (totalWeight - lift);

			thrust = numEngines * engine->getThrust(height, velocity);
			maxAcceleration = ((thrust - drag - rollingFriction) * GRAVITY) / totalWeight;                // Don't forget gravity... bc imperial!
			velocity = (maxAcceleration * TIME_STEP) + velocity;
			totalTime = totalTime + TIME_STEP;
		}

		assert(velocity > V2); // Precaution to make sure we accelerated enough
	}






// Accessors:
	double Airplane::getWeight() const {
		return totalWeight;
	}

	double Airplane::getMaxExcessPower() const {
		return maxExcessPower;
	}

	double Airplane::getVelocityMaxExcessPower() const {
		return velocityMaxExcessPower;
	}

	vector<double> Airplane::getMaxExcessPowerVector() const {
		return { velocityMaxExcessPower, maxExcessPower };
	}








// Best Gamma for ROC(max)
	/*
		RoC = (P_avail - P_req) / W 
		RoC = P_excess / W

		P = Force * velocity
		P_excess = W*v_vertical           (using all power to climb)
		RoC = v_vertical = V*sin(gamma)
		so:
			P_excess = W*V*sin(gamma)
		so:
			sin(gamma) = (P_avail - P_req) / W*V
		so:
			RoC = Vsin(gamma) = (P_excess) / W
		so:
			Max RoC when maximize P_excess

			P_excess depends on gamma bc P+req depends on gamma bc Alpha Steady level depends on gamma
	

		P_req is a function of V, h, gamma

		v_vertical = V*sin(gamma)
	
	*/

	/*
		Psuedocode for this:

		gamma = 0;
		gamma_max = 20;
		
		while(gamma_max - gamma > 0){
		P_mid = P_max((gamma + gamma_max) / 2)     // Need to write a function that just returns the max Excess power for a given gamma at a given height
		P_left = P_max(gamma)
		P_right = P_max(gamm_max)

		if (P_left > P_mind){
			gamma_max = gamma_mid;
		} else if (P_right > P_mid){
			gamma = gamma_mid
		} else{
			return gamma_mid
		}

		
	
	*/









}






















// Old Implementations:




	/*
		double Airplane::calcLiftCoeff(double AoA) const {
			double totalLift = 0;

			totalLift += mainWing->calcLiftCoeff(AoA);
			totalLift += HT->calcLiftCoeff(AoA);
			totalLift += fuselage->calcLiftCoeff(AoA);
			// Assume VT contributes negligble lift.

			return totalLift;
	}
	*/






	/*
		double Airplane::calcLift(double AoA, double velocity, double density) const {
		double totalLift = 0;

		totalLift += mainWing->calcLiftCoeff(AoA);
		totalLift += HT->calcLiftCoeff(AoA);
		totalLift += fuselage->calcLiftCoeff(AoA);
		// Assume VT contributes negligble lift.

		totalLift = 0.5 * density * velocity * velocity * referenceArea * totalLift;

		return totalLift;
	}
	
	
	*/