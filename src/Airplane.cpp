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








	// Unrealistic ("Best") Climb Function
		// (ROC)_max = (Excess Power)_max / Weight
			// Excess Power = Power Avaiable - Power Required
				// Power Required = Total Drag * velocity

		// Time to Climb = integral from h1 to h2 (dh / RoC)
			// ROC * change in time = dh

	double Airplane::calcBestClimbTime(double startHeight, double endHeight) const {
		double time = 0;

		while (endHeight > startHeight){


		}


		return time;
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









	vector<double> Airplane::calcDragPowerData(double gamma, double height) const {
		AtmosphereProperties Cond(height);
		vector<double> data;
		double density = Cond.getDensity();
		double kineVisc = Cond.getKinematicVisc();
		double temp = Cond.getTemperature();

		double dx = (xmax - xmin) / steps;     // 1000 steps between mach~=0 and mach~=1
		for (int i = 0; i < steps; i++) {
			double Mach = (i * dx) + xmin;
			double velocity = Mach * sqrt(1.4 * GAS_CONSTANT * temp);
			double AoA = calcSteadyClimbAoA(1.4, velocity, density);
			data.push_back(calcDrag(AoA, velocity, Mach, kineVisc, density) * velocity);
		}
		return data;
	}










	vector<double> Airplane::calcPowerAvailableData(double height) const {
		vector<double> xdata = calcPowerCurveVelocityData(height);
		vector<double> ydata;
		vector<double> powerCurve = engine->getPowerCurveFunction(height);
		AtmosphereProperties Cond(height);
		double tempature = Cond.getTemperature();
		double SLSThrust = engine->getSLSThrust();

		for (int i = 0; i < xdata.size(); i++) {
			double xtemp = xdata[i] / (sqrt(1.4 * GAS_CONSTANT * tempature));
			double ytemp = evaluateFunction(powerCurve, xtemp);
			ytemp = ytemp * numEngines * SLSThrust * xdata[i];
			ydata.push_back(ytemp);
		}
		return ydata;
	}









	// Slightly inefficent bc the calling functions create their own AtmosProp Object
	void Airplane::getPowerCurveCSV(double gamma, double height, string fileName) const {
		vector<double> xdata = calcPowerCurveVelocityData(height);
		vector<double> y1data = calcDragPowerData(gamma, height);
		vector<double> y2data = calcPowerAvailableData(height);
		saveVectorsToCSV(xdata, y1data, y2data, fileName);
		return;
	}







	void Airplane::calcAndSetPowerCurveData(double gamma, double height) {
		if (powerCurveVelocityData.empty()) {
			powerCurveVelocityData = calcPowerCurveVelocityData(height);
		}

		powerRequiredData = calcDragPowerData(gamma, height);
		powerAvailableData = calcPowerAvailableData(height);
		calcAndSetMaxExcessPower();

		return;
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