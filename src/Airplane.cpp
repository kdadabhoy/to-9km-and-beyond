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
		MTOW = totalWeight;
		N_ULT = 0;
		CL.setCL_Alpha(0);
		CL.setCL_Knott(0);
	}











	// don't do const Class& object, because I might want to modify the object in this class...
	// If I decide none of the objects get modified in this class, then add const here and the pointer won't
	// allow modificaiton to the passed object
	// Would also have to change all member vairbales to const Class*
	Airplane::Airplane(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage, double inFuelWeight, double inPayLoadWeight) {
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
		calcAndSetTotalWeight();    // Main Wing Weight depends on total weight before everything else, so need to calc that first

		if (fabs(mainWing->getWeight()) < .01) {
			calcAndSetMainWingWeight();				// Now calc main wing weight using Raymond Approx
			totalWeight += mainWing->getWeight();   // Add mainWingWeight to totalWeight
		}

		MTOW = totalWeight;
	}











	void Airplane::calcAndSetTotalWeight() {
		double total = 0.0;
		total += fuelWeight + payLoadWeight;

		if (mainWing) {
			total += mainWing->getWeight();
		}

		if (HT) {
			total += HT->getWeight();
		}

		if (VT) {
			total += VT->getWeight();
		}

		if (engine) {
			total += (engine->getWeight() * numEngines);
		}

		if (nacelle) {
			total += (nacelle->getWeight() * numEngines);
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
		double totalDragCoeff = 0;

		totalDragCoeff += mainWing->calcDragCoeff(AoA, mainWing->calcReynolds(velocity, kinematicViscosity), Mach, mainWing->calcWetRatio(referenceArea));
		totalDragCoeff += HT->calcDragCoeff(AoA, HT->calcReynolds(velocity, kinematicViscosity), Mach, HT->calcWetRatio(referenceArea));
		totalDragCoeff += VT->calcDragCoeff(AoA, VT->calcReynolds(velocity, kinematicViscosity), Mach, VT->calcWetRatio(referenceArea));
		totalDragCoeff += fuselage->calcDragCoeff(AoA, fuselage->calcReynolds(velocity, kinematicViscosity), Mach, fuselage->calcWetRatio(referenceArea));
		totalDragCoeff += numEngines * (nacelle->calcDragCoeff(AoA, nacelle->calcReynolds(velocity, kinematicViscosity), Mach, nacelle->calcWetRatio(referenceArea)));

		return totalDragCoeff;
	}












	double Airplane::calcDrag(double AoA, double velocity, double Mach, double kinematicViscosity, double density) const {
		double totalDrag = 0;

		totalDrag += mainWing->calcDragCoeff(AoA, mainWing->calcReynolds(velocity, kinematicViscosity), Mach, mainWing->calcWetRatio(referenceArea));
		totalDrag += HT->calcDragCoeff(AoA, HT->calcReynolds(velocity, kinematicViscosity), Mach, HT->calcWetRatio(referenceArea));
		totalDrag += VT->calcDragCoeff(AoA, VT->calcReynolds(velocity, kinematicViscosity), Mach, VT->calcWetRatio(referenceArea));
		totalDrag += fuselage->calcDragCoeff(AoA, fuselage->calcReynolds(velocity, kinematicViscosity), Mach, fuselage->calcWetRatio(referenceArea));
		totalDrag += numEngines * (nacelle->calcDragCoeff(AoA, nacelle->calcReynolds(velocity, kinematicViscosity), Mach, nacelle->calcWetRatio(referenceArea)));

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

		// Uses small angle approx for gamma
	double Airplane::calcBestClimbTime(double startHeight, double startVelocity, double endHeight) {
		// Implementing an accelerate/declerate in a straight line then climb at maxExcessPower
		// Might need to implement AoA < 15 deg or smt checks

		// Re-work notes: changed calcSteadyLevelAccelerationTime to return a struct
			// This is because there were cases with small wing spans, when we were getting infinite loops
			// Because we weren't able to reach the velocity we needed for maxExcessPower (not enough acceleration)
		// This results in the else loop having to call a getExcessPower for these cases when we aren't at ExcessPower (but are as close as possible)
			// Technically there might be a more optimial point, but we will ignore that case for now

		double totalTime = 0;
		double height = startHeight;
		double velocity = startVelocity;
		SteadyLevelAccelerationTimeProperties accelProperties;
		double currentExcessPower;
		bool canReachMaxVelocity = true;
		double lastMaxExcessVelcTryHeight = 0;
		double TRY_AGAIN_EXCESS_VEL_INTERVAL = 2000;     // how many ft before trying to reach maxExcessVel again (so we save some efficiency)
		// double VELOCITY_ERROR = .5; // global constant now
		double TIME_STEP = .1;							// Can Change if too inefficient


		while (height <= endHeight) {
			calcAndSetPowerCurveData(height);
			calcAndSetMaxExcessPower();

			if (fabs(velocity - velocityMaxExcessPower) > VELOCITY_ERROR && canReachMaxVelocity) {
				// calcSteadyLevelAccel also updates Weight
				accelProperties = calcSteadyLevelAccelerationTime(velocity, velocityMaxExcessPower, height);
				totalTime += accelProperties.timeTaken;
				velocity = accelProperties.finalVelocity;

				canReachMaxVelocity = accelProperties.canReachFinalVel;

				if (!canReachMaxVelocity) {
					// Will only try to reach maxExcessVel after climbing 500ft or smt
					// This if block is used for that purpose
					lastMaxExcessVelcTryHeight = height;
				}

			} else {
				currentExcessPower = calcExcessPower(velocity); // can't juse use maxExcess power.. it typically is... but there are cases when it's not

				height += (currentExcessPower / totalWeight) * TIME_STEP; // small angle approx method
				totalTime += TIME_STEP;

				if (fabs(currentExcessPower) < 0.001) {
					// Plane can't reach 8km case
					totalTime = 1e10;
					cout << "Plane with Wing Span (ft): " << mainWing->getSpan() <<
						" and taper: " << mainWing->getTaperRatio() << " could not reach 9km. "
						<< "total time for this plane set to a very large number!" << endl;
					break;

				}


				if (((height - lastMaxExcessVelcTryHeight) > TRY_AGAIN_EXCESS_VEL_INTERVAL) && !canReachMaxVelocity) {
					// Want to try reach maxExcessVel again
					canReachMaxVelocity = true; 
				}


				// Weight Loss Consideration
				totalWeight -= numEngines * engine->calcFuelLoss2(TIME_STEP, height, velocity);

				/*
					// If you want to see how gamma changes during flight
					double gamma = asin(maxExcessPower / (totalWeight * velocityMaxExcessPower));
					cout << "gamma (deg): " << gamma * 180 / 3.1415 << endl;
				*/
			}

		}


		return totalTime;
	}













	// Only evalutes Power Curves when height changes by 500
	double Airplane::calcBestClimbTimeApprox(double startHeight, double startVelocity, double endHeight, double heightSteps) {
		// Implementing an accelerate/declerate in a straight line then climb at maxExcessPower
		double totalTime = 0;
		double height = startHeight;
		double velocity = startVelocity;
		SteadyLevelAccelerationTimeProperties accelProperties;
		double currentExcessPower;
		bool canReachMaxVelocity = true;
		double lastMaxExcessVelcTryHeight = 0;
		double TRY_AGAIN_EXCESS_VEL_INTERVAL = 2000;     // how many ft before trying to reach maxExcessVel again (so we save some efficiency)
		//double VELOCITY_ERROR = .5; // global const now
		double TIME_STEP = .1;                           // Can Change if too inefficient


		double oldHeight = startHeight;
		double HEIGHT_STEP = heightSteps;                  // 0 = same as other function. As you increase this, 

		calcAndSetPowerCurveData(height);
		calcAndSetMaxExcessPower();
		while (height <= endHeight) {

			if (height - oldHeight >= HEIGHT_STEP) {
				calcAndSetPowerCurveData(height);	// Small angle approx, gets me my power curve
				calcAndSetMaxExcessPower();			// Gets me my max Excess Power and what velocity it is at
				oldHeight = height;
			}

			if (fabs(velocity - velocityMaxExcessPower) > VELOCITY_ERROR && canReachMaxVelocity) {
				// calcSteadyLevelAccel also updates Weight
				accelProperties = calcSteadyLevelAccelerationTime(velocity, velocityMaxExcessPower, height);
				totalTime += accelProperties.timeTaken;
				velocity = accelProperties.finalVelocity;

				canReachMaxVelocity = accelProperties.canReachFinalVel;

				if (!canReachMaxVelocity) {
					// Will only try to reach maxExcessVel after climbing 500ft or smt
					// This if block is used for that purpose
					lastMaxExcessVelcTryHeight = height;
				}

			} else {
				currentExcessPower = calcExcessPower(velocity); // can't juse use maxExcess power.. it typically is... but there are cases when it's not

				height += (currentExcessPower / totalWeight) * TIME_STEP; // small angle approx method
				totalTime += TIME_STEP;

				if (fabs(currentExcessPower) < 0.001) {
					// Plane can't reach 8km case
					totalTime = 1e10;
					cout << "Plane with Wing Span (ft): " << mainWing->getSpan() <<
						" and taper: " << mainWing->getTaperRatio() << " could not reach 9km. "
						<< "total time for this plane set to a very large number!" << endl;
					break;

				}


				if (((height - lastMaxExcessVelcTryHeight) > TRY_AGAIN_EXCESS_VEL_INTERVAL) && !canReachMaxVelocity) {
					// Want to try reach maxExcessVel again
					canReachMaxVelocity = true; 
				}

				// Weight Loss Consideration
				totalWeight -= numEngines * engine->calcFuelLoss2(TIME_STEP, height, velocity);
			}
		}
		return totalTime;
	}







	// Calcs the time to 9km (accurately) from h=0 to h = 9km + startHeight (accounts for possibility of not starting at sea level)
	double Airplane::calcBestTimeTo9km(double startHeight, double takeOffEndHeight) {
		double END_HEIGHT = 29527.6 + startHeight; // 9km in ft
		double velocity = 0;                       // Starting from rest

		TakeoffProperties takeoffProperties = calcTakeoffPropertites(startHeight, takeOffEndHeight, velocity, totalWeight);

		double totalTime = takeoffProperties.timeTaken;
		velocity = takeoffProperties.finalVelocity;
		takeOffEndHeight = takeoffProperties.height;

		// Weight Loss Consideration
		totalWeight = takeoffProperties.finalWeight;

		totalTime += calcBestClimbTime(takeOffEndHeight, velocity, END_HEIGHT);

		return totalTime; // Returns time in seconds
	}







	// Calcs the time to 9km (approx) from h=0 to h = 9km + startHeight (accounts for possibility of not starting at sea level)
	double Airplane::calcBestTimeTo9kmApprox(double startHeight, double takeOffEndHeight, double heightSteps) {
		double END_HEIGHT = 29527.6 + startHeight; // 9km in ft
		double velocity = 0;                       // Starting from rest

		TakeoffProperties takeoffProperties = calcTakeoffPropertites(startHeight, takeOffEndHeight, velocity, totalWeight);

		double totalTime = takeoffProperties.timeTaken;
		velocity = takeoffProperties.finalVelocity;
		takeOffEndHeight = takeoffProperties.height;

		// Weight Loss Consideration
		totalWeight = takeoffProperties.finalWeight;

		totalTime += calcBestClimbTimeApprox(takeOffEndHeight, velocity, END_HEIGHT, heightSteps);

		return totalTime; // Returns time in seconds
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
	Airplane::SteadyLevelAccelerationTimeProperties Airplane::calcSteadyLevelAccelerationTime(double startVelocity, double finalVelocity, double height) {
		// Re-work Notes: changed return type to a struct, because we had infinite loop cases
			// If we get to acceleration = 0, we won't be able to reach maxExcessPower, so we need to return the finalVel and timeTaken

		double totalTime = 0;
		double velocity = startVelocity;
		SteadyLevelAccelerationTimeProperties returnStruct;
		returnStruct.canReachFinalVel = true;
		double MAX_ACCLERATION_CUTOFF = .5; // When to say we can't reach finalVel... smaller is more accurate... but adds a lot of non climb time

		AtmosphereProperties Cond(height);
		double temp = Cond.getTemperature();
		double kineVisc = Cond.getKinematicVisc();
		double density = Cond.getDensity();
		double Mach, thrust, AoA, drag, maxAcceleration, engineFactor;

		double TIME_STEP = 0.05;        // Can Change if too inefficient
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
				// Set this to 0% for best possible time... but real life is ~20% of thrust being minimum.. but we are climbing so we technically just adjust pitch...
			}

			thrust = numEngines * engine->getThrust(height, velocity) * engineFactor;
			maxAcceleration = ((thrust - drag) * GRAVITY) / totalWeight;                // Don't forget gravity... bc imperial!
			velocity = (maxAcceleration * TIME_STEP) + velocity;
			totalTime = totalTime + TIME_STEP;
			velDifference = finalVelocity - velocity;

			if (fabs(maxAcceleration) < .10) {
				// We can't reach finalVelocity case
				returnStruct.canReachFinalVel = false;
				break;
			}

			// Weight Loss Consideration
			//totalWeight -= numEngines * engine->calcFuelLoss2(TIME_STEP, height, velocity); // this is breaking like 40ft span, but helping 46ft

			assert(fabs(AoA) < (20 * 3.1415) / 180);                                    // Just a precaution to make sure AoA never goes above 20 deg (optimisitic)
		}


		returnStruct.finalVelocity = velocity;
		returnStruct.timeTaken = totalTime;
		return returnStruct; 
	}










	// Takeoff Functions

	// *** Need to account for weight loss ***
	// See physics derivation in notes
	// Old - doesnt account for totalWeightLoss
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
			maxAcceleration = ((thrust - drag - rollingFriction) * GRAVITY) / totalWeight;  // Don't forget gravity... bc imperial!
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











	// Note: a common FAA endheight is around 500 ft... the best endHeight for climb is 0 ft (with the way the program is written)
		// Technically 0 ft shouldn't be the best... unless you were somehow able to get rid of your wheels...
		// But with the assumption you are able to steady level accelerate at 0 ft... and ignoring ground effect... 0 ft is the best
	// Assumes startingWeight of aircraft = it's stored totalWeight (just know that when calling the function... it's true 99% of the time)
	Airplane::TakeoffProperties Airplane::calcTakeoffPropertites(double startHeight, double endHeight, double startVelocity, double startWeight) {
		double weight = startWeight;
		// Runway Stuff
		TakeoffProperties runwayProp = calcEndRunwayAirplaneProperties(startHeight, startVelocity, weight);
		double timeSoFar = runwayProp.timeTaken;
		double velocity = runwayProp.finalVelocity;
		weight = runwayProp.finalWeight;

		// Climbing after Runway
		double height = startHeight;
		AtmosphereProperties Cond(height);
		double TIME_STEP = 0.05;                 // Can Change if too inefficient

		while (height < endHeight) {
			double density = Cond.getDensity();
			calcAndSetPowerCurveData(height);
			double excessPower = calcExcessPower(velocity);
			height += (excessPower / weight) * TIME_STEP; // small angle approx method
			timeSoFar += TIME_STEP;

			// Weight Loss Consideration
			weight -= numEngines * engine->calcFuelLoss2(TIME_STEP, height, velocity);

			// Just a precaution to make sure AoA never goes above 15 deg (optimisitic)
			double AoA = calcSteadyClimbAoAApprox(velocity, density);
			assert(fabs(AoA) < (15 * 3.1415) / 180);
		}


		TakeoffProperties returnStrut;
		returnStrut.height = height;
		returnStrut.timeTaken = timeSoFar;
		returnStrut.finalVelocity = velocity;
		returnStrut.finalWeight = weight;
		return returnStrut;
	}










	// *** Need to account for weight loss ***

	// Incomplete
	// Assumes we have enough control authority to takeoff, if we can reach V2
	Airplane::TakeoffProperties Airplane::calcEndRunwayAirplaneProperties(double height, double startVelocity, double startWeight) const {

		double AoA_Stall = 15 * pi / 180;      // Just using a realistic, arbirtary AoA rn
		double CL_Stall = calcLiftCoeff(AoA_Stall);

		AtmosphereProperties Cond(height);
		double density = Cond.getDensity();
		double temp = Cond.getTemperature();
		double kineVisc = Cond.getKinematicVisc();

		double V2_min = 1.2 * sqrt((2 * startWeight) / (density * CL_Stall * mainWing->getArea()));

		double weight = startWeight;
		double velocity = startVelocity;
		double timeSoFar = 0;
		double Mach, thrust, AoA, drag, lift, rollingFriction, maxAcceleration;
	
		double ROLLING_FRIC_COEFF = .02; // A realistic approx
		double TIME_STEP = 0.05;        // Can Change if too inefficient

		while (velocity < V2_min) {
			Mach = calcMach(velocity, temp);
			AoA = 2 * pi / 180;				 // An Assumption, it is very close to 0... while on runway
			drag = calcDrag(AoA, velocity, Mach, kineVisc, density);
			lift = calcLift(AoA, velocity, density);
			rollingFriction = ROLLING_FRIC_COEFF * (weight - lift);

			thrust = numEngines * engine->getThrust(height, velocity);
			maxAcceleration = ((thrust - drag - rollingFriction) * GRAVITY) / weight;   // Don't forget gravity... bc imperial!
			velocity = (maxAcceleration * TIME_STEP) + velocity;
			timeSoFar += TIME_STEP;

			// Accounting for Weight Loss
			weight -= numEngines * engine->calcFuelLoss2(TIME_STEP, height, velocity);

		}

		assert(velocity > V2_min); // Precaution to make sure we accelerated enough

		TakeoffProperties returnStrut;
		returnStrut.finalVelocity = velocity;
		returnStrut.timeTaken = timeSoFar;
		returnStrut.finalWeight = weight;
		return returnStrut;
	}












	// Wing Weight Approx (Raynmond Cargo/Transport Approx)
	// (w/ FAR 25 Cert for n and a 1.5 safety factor)
	// weight = K * pow(totalWeight * n_ult, 0.557)
	void Airplane::calcAndSetMainWingWeight() {
		double wingArea = mainWing->getArea() * (1 - PERCENT_CONTROL_SURFACE_AREA); // wingArea = totalArea - control surface area
		double aspectRatio = mainWing->getAspectRatio();
		double taperRatio = mainWing->getTaperRatio();
		double controlArea = mainWing->getArea() * PERCENT_CONTROL_SURFACE_AREA;
		double sweepAngle = mainWing->getSweepAngleRad();
		Airfoil* mainAirfoil = mainWing->getAirfoil();
		double thicknessRatio = mainAirfoil->getThicknessRatio();  // t/c
		double weightEverythingExceptWing = totalWeight;

		double K = RAYMOND_CST * pow(wingArea, 0.649) * sqrt(aspectRatio) * pow(thicknessRatio, -0.4)
			* pow(1 + taperRatio, 0.1) * (1 / cos(sweepAngle)) * pow(controlArea, 0.1);


		int MAX_ITERATIONS = 500;             // Change as needed
		double TOLERANCE = 0.1;               // Change as needed

		double N_ult = MIN_LIMIT_LOAD_FACTOR * LOAD_SAFETY_FACTOR;
		double prev_N_ult = 0;

		double wingWeight = weightEverythingExceptWing * .10;   // 10% of everything else is a good starting point for iteration
		double totalWeightGuess;
		int outer_iter = 0;

		double prevWingWeight = 0;
		int inner_iter = 0;

		while (outer_iter < MAX_ITERATIONS && fabs(N_ult - prev_N_ult) >= TOLERANCE) {
			prev_N_ult = N_ult;

			double prevWingWeight = 0;
			int inner_iter = 0;

			while (inner_iter < MAX_ITERATIONS && fabs(wingWeight - prevWingWeight) >= TOLERANCE) {
				prevWingWeight = wingWeight;
				totalWeightGuess = weightEverythingExceptWing + wingWeight;
				wingWeight = K * pow(totalWeightGuess * N_ult, 0.557);
				inner_iter++;
			}

			N_ult = (2.1 + (24000 / (wingWeight + weightEverythingExceptWing))) * LOAD_SAFETY_FACTOR;

			if (N_ult < MIN_LIMIT_LOAD_FACTOR * LOAD_SAFETY_FACTOR) {
				N_ult = MIN_LIMIT_LOAD_FACTOR;
			} else if (N_ult > MAX_LIMIT_LOAD_FACTOR * LOAD_SAFETY_FACTOR) {
				N_ult = MAX_LIMIT_LOAD_FACTOR;
			}

			outer_iter++;
		}

		N_ULT = N_ult;
		mainWing->setWeight(wingWeight * SMUDGE_FACTOR);
		return;
	}








	// Wing Feasability Functions:


	double Airplane::calcLimitLift() const {
		// L_lim = n_lim * MTOW 
		return (N_ULT / LOAD_SAFETY_FACTOR) * MTOW;
	}






	double Airplane::calcRootLimitMoment() const {
		double limitLift = calcLimitLift();
		return mainWing->calcRootMoment(limitLift);
	}







	// MIT way
	double Airplane::calcRootLimitStress() const {
		// sigma_limit = M_root,_limit * c / I
		double sigma = (calcRootLimitMoment() * mainWing->calc_C_ForRootStress()) / mainWing->calcRootInertiaEstimate(); // psf
		return sigma * PSF_TO_PSI * PSI_TO_KSI;  // returns ksi
	}









/*
	// Cap Way
	double Airplane::calcRootLimitStress() const {
		// sigma_limit = M_root,_limit * c / I
		// c = distance from neutral axis to the outermost fiber of the cross section 
		double sigma = calcRootLimitMoment() / mainWing->calc_C_ForRootStress();
		return sigma * PSF_TO_PSI * PSI_TO_KSI;  // returns ksi
	}
*/





	







	/* // Old
		double Airplane::calcRootLimitStress() const {
		// sigma_limit = M_root,_limit * c / I
		// c = distance from neutral axis to the outermost fiber of the cross section 

		double sigma = calcRootLimitMoment() * mainWing->calc_C_ForRootStress() / mainWing->calcRootInertiaEstimate(); // psf
		return sigma * PSF_TO_PSI * PSI_TO_KSI;  // returns ksi
	}
	
	
	*/





/*
	// Maybe dumb
	double Airplane::calcMinSpanNeeded(double maxRootStressKSI) const {
		// sigma_limit = M_root,_limit * c / I
		// c = distance from neutral axis to the outermost fiber of the cross section
		double sigma = calcRootLimitMoment() * mainWing->calc_C_ForRootStress() / mainWing->calcRootInertiaEstimate(); // psf
		return sigma * PSF_TO_PSI * PSI_TO_KSI;  // returns ksi
	}
*/








	bool Airplane::isWingPossible() const {
		// Need to see if Wing can
		// 1) Material can withstand the maximum root load
		// 2) If we have enough span
		// 3) Actually Takeoff (?) - not implemented currently
		bool canWithStandLoad = false;
		bool canTakeoff = true; // delete / set to false... set to true for testing purposes

		double maxRootStress = calcRootLimitStress();
		double maxLift = calcLimitLift();
		double inertia = mainWing->calcRootInertiaEstimate();
		double c_cst = mainWing->calc_C_ForRootStress();
		double sigma_needed_ft = maxRootStress * 1000 * 144;
		double taper = mainWing->getTaperRatio();
		double spanNeeded = (6 * (1+taper) * sigma_needed_ft * inertia) / (maxLift * c_cst);

		cout << "Span needed: " << spanNeeded << endl; // del

		double currentSpan = mainWing->getSpan();

		if (maxRootStress <= SIGMA_YIELD_COMPOSITE && spanNeeded >= currentSpan) {
			canWithStandLoad = true;

			if (canTakeoff) {
				return true;
			}

		}

		return false;
	}

















	// Mutators:
	
	// This function completely resets the airplane with the new wing (weight goes back to MTOW)
	void Airplane::setMainWing(Wing& inWing) {
		assert(mainWing != nullptr);			// Precaution

		mainWing = &inWing;
		referenceArea = mainWing->getArea();    // Reference Area depends on mainWing
		calcAndSetLiftCoeff();					// Lift Coeff depends on mainWing
		calcAndSetTotalWeight();				// Main Wing Weight depends on total weight before everything else

		if (fabs(mainWing->getWeight()) < .01) {
			calcAndSetMainWingWeight();				// Now calc main wing weight using Raymond Approx
			totalWeight += mainWing->getWeight();   // Add mainWingWeight to totalWeight
		} 

		MTOW = totalWeight;
		return;
	}









	void Airplane::setMainWingWeight(double inWeight) {
		assert(mainWing != nullptr);		// Precaution
		mainWing->setWeight(inWeight);
		calcAndSetTotalWeight();
		MTOW = totalWeight;
		return;
	}












	// Accessors:
	double Airplane::getWeight() const {
		return totalWeight;
	}








	double Airplane::getMainWingWeight() const {
		return mainWing->getWeight();
	}









	double Airplane::getMTOW() const {
		return MTOW;
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


















/*
	Notes:

	// Climb Functions

		(ROC)_max = (Excess Power)_max / Weight
		Excess Power = Power Avaiable - Power Required
		Power Required = Total Drag * velocity

		Time to Climb = integral from h1 to h2 (dh / RoC)
		ROC * change in time = dh
		RoC = (P_avail - P_req) / weight
		dh = (RoC)dt







	// Wing Weight Approx (Raynmond w/ FAR 25 Cert for n and a 1.5 safety factor)

		Equation:
		weight = RAYMOND_CST * pow(totalWeight * n_ult, 0.557) * pow(wingArea, 0.649)
			* sqrt(aspectRatio) * pow(thicknessRatio, -0.4) * pow(1 + taperRatio, 0.1)
			* (1 / cos(sweepAngle)) * pow(controlArea, 0.1);

		weight = K * pow(totalWeight * n_ult, 0.557)

		totalWeight = wingWeight + everything else
		So need to solve iteratively




	// calcTakeoffTime():
		This function assumes we start climbing at V2 = 1.2V_stall = 1.2 * sqrt((2*W) / (rho*CL_max*S))
		And we climb until we reach 500 ft.. which is on the low end of FAA regs
			I have doubts that this is the best way to takeoff for a time-to-climb...
			but for realism and the purpose of integrating lec content into this program
			we will take this approach

		V2 = 1.2V_stall = 1.2 * sqrt((2*W) / (rho*CL_max*S))
		Rolling Resistance = mu*(W-(q*S*C_L))
			 mu ~= .02
		 a  = F/m
			Force = Thrust - Drag - Rolling Resistance
			F = T - qSC_D - mu*(W-(q*S*C_L))
			m = W/g


	//

*/












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
