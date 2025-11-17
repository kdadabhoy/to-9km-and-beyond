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

		double dx = (xmax - xmin) / steps;   // 1000 steps between mach~=0 and mach~=1
		for (int i = 0; i < steps; i++) {
			double Mach = (i * dx) + xmin;
			data.push_back(Mach);
		}
		return data;
	}







	vector<double> Airplane::calcPowerCurveVelocityData(double height) const {
		vector<double> data;
		AtmosphereProperties Cond(height);
		double temp = Cond.getTemperature();

		double dx = (xmax - xmin) / steps;   // 1000 steps between mach~=0 and mach~=1
		for (int i = 0; i < steps; i++) {
			double Mach = (i * dx) + xmin;
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








	double Airplane::calcExcessPower(double velocity) const {
		double dx = (xmax - xmin) / steps;   // Power Curve velocities have known spacing
		double point = (velocity - xmin) / dx;
		int lowerIndex = static_cast<int>(floor(point));
		int upperIndex = lowerIndex + 1;

		// Maybe possible to do without this interpolation method bc evenly spaced...
		double powerAvail = linearInterpolate(point, powerCurveVelocityData[lowerIndex], powerAvailableData[lowerIndex], powerCurveVelocityData[upperIndex], powerAvailableData[upperIndex]);
		double powerReq = linearInterpolate(point, powerCurveVelocityData[lowerIndex], powerRequiredData[lowerIndex], powerCurveVelocityData[upperIndex], powerRequiredData[upperIndex]);

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

		double dx = (xmax - xmin) / steps;     // 1000 steps between mach~=0 and mach~=1
		for (int i = 0; i < steps; i++) {
			double Mach = (i * dx) + xmin;
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

		double dx = (xmax - xmin) / steps;     // 1000 steps between mach~=0 and mach~=1
		for (int i = 0; i < steps; i++) {
			double Mach = (i * dx) + xmin;
			double velocity = Mach * sqrt(1.4 * GAS_CONSTANT * temp);
			double AoA = calcSteadyClimbAoAApprox(velocity, density);

			/*
			cout << "Height: " << height << endl; // delete
			cout << "AoA: " << AoA * 180 / 3.1415 << " velocity: " << velocity << endl; // delete
			*/

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
		// Might need to implement AoA < 15 deg or smt checks
	double Airplane::calcBestClimbTime(double startHeight, double startVelocity, double endHeight) {
		// Implementing an accelerate/declerate in a straight line then climb at maxExcessPower
		double totalTime = 0;
		double height = startHeight;
		double velocity = startVelocity;
		double VELOCITY_ERROR = .5;						// Can Change if too inefficient
		double TIME_STEP = .1;                      // Can Change if too inefficient


		while (height <= endHeight) {
			calcAndSetPowerCurveData(height);
			calcAndSetMaxExcessPower();

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








// Takeoff Functions
	double Airplane::calcTakeoffTime() const {
		return 0;
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