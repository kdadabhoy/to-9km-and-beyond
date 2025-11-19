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

// Can rewrite takeoff property function with a strut 

namespace airplane {
	class Airplane {
	public:
		Airplane();
		Airplane(Wing& inWing, Wing& inHT, Wing& inVT, CF34_3B1& inEngine, Nacelle& inNacelle, Fuselage& inFuselage, double inFuelWeight, double inPayLoadWeight);

	// Useful Functions:
		double calcMach(double velocity, double temp) const;

		double calcDragCoeff(double AoA, double velocity, double Mach, double kinematicViscosity) const;				// AoA in Rad
		double calcDrag(double AoA, double velocity, double Mach, double kinematicViscosity, double density) const;     // AoA in Rad

		double calcLiftCoeff(double AoA) const;																			// AoA in Rad
		double calcLift(double AoA, double velocity, double density) const;												// AoA in Rad

	// Power Curve
		void getPowerCurveCSV(double gamma, double height, string fileName) const;           // fileName should have ".csv", no small angle approx
		void getPowerCurveCSV(double height, string fileName) const;						 // fileName should have ".csv", small angle approx 


	// Takeoff Functions
		// Probably create a struct and return a struct instead of passing by reference
		void calcTakeoffPropertites(double height, double& endHeight, double& totalTime, double& velocity); // Use this one..
		double calcTakeoffTime(double height, double endHeight);                                           // Just returns time... less useful


	// Climb Functions
		double calcBestClimbTime(double startHeight, double startVelocity, double endHeight);  
		double calcBestClimbTimeApprox(double startHeight, double startVelocity, double endHeight);   // doesnt do power curve at every height... just some

		double calcSteadyClimbAoA(double gamma, double velocity, double density) const;        // Gamma in degrees, Returns AoA in rad
		double calcSteadyClimbAoAApprox(double velocity, double density) const;				   // Small angle approx, so cos(gamma) = 1
		

	// Steady Level Flight Functions (L = W)
		double calcSteadyLevelAoA(double velocity, double density) const;
		double calcSteadyLevelAccelerationTime(double startVelocity, double finalVelocity, double height);




	// Accessors:
		double getWeight() const;
		double getMaxExcessPower() const;					// Assumes power curve is set 
		double getVelocityMaxExcessPower() const;           // Assumes power curve is set
		vector<double> getMaxExcessPowerVector() const;     // Assumes power curve is set


	private:
	// Must take in:
		Wing* HT;                // Full HT (Full Span / Planform)
		Wing* VT;                // Full VT (Full Span / Planform)
		Wing* mainWing;          // Full mainWing (Full Span / Planform)
		CF34_3B1* engine;        // One engine (Class assumes plane has 2 identical engines)
		Nacelle* nacelle;        // One nacelle (Class assumes plane has 2 identical nacelles)
		Fuselage* fuselage;      // One fuselage
		double payLoadWeight;    // lbm
		double fuelWeight;       // lbm


	// Class automatically assigns:
		double referenceArea;    // ft^2, Class assigns this the area of the main wing
		double totalWeight;      // lbm,  Class calculates this
		LiftCoeff CL;            // CL of the whole plane


	// Power Curve Data:
		vector<double> powerCurveVelocityData;
		vector<double> powerRequiredData;
		vector<double> powerAvailableData;
		double maxExcessPower = 0;
		double velocityMaxExcessPower = 0;


	// Wing Weight Constants (Assumptions)
		// FAR 25 cert; n = 2.1 + (24000 / MTOW), 2.5 <= n <= 3.8
		static constexpr double RAYMOND_CST = .0051;			          // Raymond constat in Cargo/Transport Weight Eq'n
		static constexpr double MIN_LIMIT_LOAD_FACTOR = 2.5;    
		static constexpr double MAX_LIMIT_LOAD_FACTOR = 3.8;	

		static constexpr double LOAD_SAFETY_FACTOR = 1.5;				  // Typical safety factor 
		static constexpr double SMUDGE_FACTOR = 0.85;                     // Our wing is an advanced composite... obviously! (Table 15.4 from Raymond book)
		static constexpr double PERCENT_CONTROL_SURFACE_AREA = 0.10;      // % Control Surface Area of main Wing (Approx 10%)

		static constexpr double MAX_MTOW = 35000 * 2.20462;               // lbms, Project specifications / Max limit for this competition
		static constexpr double MIN_MTOW = 25000 * 2.20462;               // lbms, Project specifications / Min limit for this competition


	// Power Function's Curve Constants
		static constexpr double X_MIN = .01;   // This xmin and xmax are used to space out Mach
		static constexpr double X_MAX = .975;
		const static int STEPS = 1000;


	// Constants
		const static int numEngines = 2;
		static constexpr double GAS_CONSTANT = 1716;
		static constexpr double pi = 3.141592653589;
		static constexpr double GRAVITY = 32.2;





	// calcAndSet Functions:
		void calcAndSetTotalWeight();                                   // Used in constructors
		void calcAndSetLiftCoeff();										// Needed for SteadyClimbAoA... sets Cl
		void calcAndSetMaxExcessPower();					            // Sets maxExcessPower and velocityExcessPower (assumes priv memb vars are accurate)
		void calcAndSetPowerCurveData(double gamma, double height);		// No small angle approx, Used for setting all data
		void calcAndSetPowerCurveData(double height);					// Small angle approx, Used for setting all data
		void calcAndSetMainWingWeight();								// Raymond Cargo Method, with some assumptions



	// Power Curve Functions
		vector<double> calcPowerCurveMachData() const;						      // Will Always be the same
		vector<double> calcPowerCurveVelocityData(double height) const;			  // Power Curve needs velocity
		vector<double> calcPowerAvailableData(double height) const;
		vector<double> calcPowerRequiredData(double height) const;                // Small Angle Approx (cos(gamma) ~= 1)

		// No Small Angle Approx:	
		vector<double> calcPowerRequiredData(double gamma, double height) const;   // Gamma in degrees, generates 1000 evenly spaced data points between Mach 0 and 1
		

	// Climb Functions
		double calcExcessPower(double velocity) const;				                           // Assumes Power Curve member vars are set
		double calcSteadyClimbAoA(double gamma, double velocity, double density) const;        // Gamma in degrees, Returns AoA in rad
		double calcSteadyClimbAoAApprox(double velocity, double density) const;				   // Small angle approx, so cos(gamma) = 1


	// Takeoff Functions:
		// Should prob turn this into a struct
		void calcEndRunwayAirplaneProperties(double height, double& totalTime, double& velocity) const;   // Modifies the passed by reference totalTime and velocity to what it is 																									        // When the airplane leaves the runway 
																										  // Should always pass in velocity = 0 (start from rest)
																										  // Should always pass in totalTime = 0 (time starts at 0)


	};




}


#endif












/*
	Notes:

		// Think about adding for efficiency:
		// vector<double> calcExcessPower(double velocity, double height) const;	  // Doesn't assume Power Curve member vars are set
		// void calcAndSetPowerRequiredData(double gamma, double height);			// Used if you only need to change Power Required (bc changed gamma)





*/