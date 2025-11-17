#ifndef AIRPLANE_H
#define AIRPLANE_H
#include <string>
using std::string;
#include <vector>
using std::vector;
#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/CF34_3B1.h"
#include "to-9km-and-beyond/Nacelle.h"
#include "to-9km-and-beyond/Fuselage.h"


// Assumes that plane is symmetric and has 2 engines (wings are symmetrical)
// Assumes VT makes negligible lift and drag
// Can re-write some functions to take in height instead so I can use AtmosphereProp Class behind the scences

namespace airplane {
	class Airplane {
	public:
		Airplane();
		Airplane(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage, double inFuelWeight, double inPayLoadWeight);

		double calcMach(double velocity, double temp) const;

		double calcDragCoeff(double AoA, double velocity, double Mach, double kinematicViscosity) const;				// AoA in Rad
		double calcDrag(double AoA, double velocity, double Mach, double kinematicViscosity, double density) const;     // AoA in Rad

		double calcLiftCoeff(double AoA) const;																			// AoA in Rad
		double calcLift(double AoA, double velocity, double density) const;												// AoA in Rad

	// Power Curve
			// Maybe add the maxExcessPower and where it occurs to the output
		void getPowerCurveCSV(double gamma, double height, string fileName) const;           // fileName should have ".csv", no small angle approx
		void getPowerCurveCSV(double height, string fileName) const;						 // fileName should have ".csv", small angle approx 


	// Takeoff Functions
			// Need final height, final time, final AoA, final gamma, and total time
		double calcTakeoffTime() const;


	// Climb Functions
		double calcBestClimbTime(double startHeight, double startVelocity, double endHeight);  // Most important function might need to add start AoA and gamma
		double calcSteadyClimbAoA(double gamma, double velocity, double density) const;       // Gamma in degrees, Returns AoA in rad
		double calcSteadyClimbAoAApprox(double velocity, double density) const;				   // Small angle approx, so cos(gamma) = 1
		
	// Steady Level Flight Functions (L = W)
		double calcSteadyLevelAoA(double velocity, double density) const;
		double calcSteadyLevelAccelerationTime(double startVelocity, double finalVelocity, double height);



	// Accessors:
		double getWeight() const;
		double getMaxExcessPower() const;
		double getVelocityMaxExcessPower() const;
		vector<double> getMaxExcessPowerVector() const;


	// Should be private (used for testing:
		/*
		void calcAndSetPowerCurveData(double gamma, double height);					// No small angle approx, Used for setting all data
		void calcAndSetPowerCurveData(double height);								// Small angle approx, Used for setting all data
		*/


	private:
		double totalWeight;      // Class calculates this
		double referenceArea;    // Class assigns this the area of the main wing
		Wing* HT;                // Full HT (Full Span / Planform)
		Wing* VT;                // Full VT (Full Span / Planform)
		Wing* mainWing;          // Full mainWing (Full Span / Planform)
		CF34_3B1* engine;        // One engine (Class assumes plane has 2 identical engines)
		Nacelle* nacelle;        // One nacelle (Class assumes plane has 2 identical nacelles)
		Fuselage* fuselage;      // One fuselage
		double payLoadWeight;    //
		double fuelWeight;       // Weight of Fuel
		LiftCoeff CL;           // One CL for the whole plane

	// Power Curve
		vector<double> powerCurveVelocityData;
		vector<double> powerRequiredData;
		vector<double> powerAvailableData;
		double maxExcessPower = 0;
		double velocityMaxExcessPower = 0;


	// Constants
		const static int numEngines = 2;
		static constexpr double GAS_CONSTANT = 1716;
		static constexpr double pi = 3.141592653589;
		static constexpr double GRAVITY = 32.2;


	// Power Function's Curve Constants
		static constexpr double xmin = .01;
		static constexpr double xmax = .975;
		const static int steps = 1000;






	// calcAndSet Functions:
		void calcAndSetTotalWeight();
		void calcAndSetLiftCoeff();													// Needed for SteadyClimbAoA... sets Cl
		void calcAndSetMaxExcessPower();					                        // Sets maxExcessPower and velocityExcessPower (assumes priv memb vars are accurate)
		void calcAndSetPowerCurveData(double gamma, double height);					// No small angle approx, Used for setting all data
		void calcAndSetPowerCurveData(double height);								// Small angle approx, Used for setting all data



	// Power Curve Functions
		vector<double> calcPowerCurveMachData() const;						      // Will Always be the same
		vector<double> calcPowerCurveVelocityData(double height) const;			  // Power Curve needs velocity
		vector<double> calcPowerAvailableData(double height) const;

		// No Small Angle Approx:	
		vector<double> calcPowerRequiredData(double gamma, double height) const;      //Gamma in degrees, generates 1000 evenly spaced data points between Mach 0 and 1
		
		// Small Angle Approx (cos(gamma) ~= 1)
		vector<double> calcPowerRequiredData(double height) const;




	// Climb Functions
		double calcExcessPower(double velocity) const;				      // Assumes Power Curve member vars are set


		// Think about adding for efficiency:
		// vector<double> calcExcessPower(double velocity, double height) const;	  // Doesn't assume Power Curve member vars are set
		// void calcAndSetPowerRequiredData(double gamma, double height);			// Used if you only need to change Power Required (bc changed gamma)

	};




}


#endif