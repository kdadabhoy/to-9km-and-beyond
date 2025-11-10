#ifndef AIRFOIL_H
#define AIRFOIL_H
#include "to-9km-and-beyond/LiftCoeff.h"
#include <string>
// Intended to work with 4 series NACA airfoils
	// Assumes slope of 2D Cl = 2*pi
// Prob should add assignment operator at some point

/* Takeaways:
		I only really care about 
			Cl_alpha, which = 2pi = 6.28rad (good enough approx)
			alphaZeroLift = ??? need to find equation and implement it at some point
	
 */

namespace airplane {
	class Airfoil {
	public:
		Airfoil();
		Airfoil(const std::string& inNACA);
		Airfoil(const std::string& inNACA, double inAlphaZeroLift);
		Airfoil(const std::string& inNACA, double inAlphaZeroLift, double inCl0);
		double getCl_deg(double inAlphaDeg) const;
		double getCl_rad(double inAlphaRad) const;

		double getCl_AlphaTerm() const;				// Radians
		double getCl_KnottTerm() const;             // No one really cares about this
		double getCl_alphaZeroLift() const;			// Radians




	private:
		std::string NACA;               
		double maxCamber;									// Maximum camber (% of chord), 1st digit
		double posMaxCamber;								// Location of maximum camber (% of chord), 2nd digit
		double maxThickness;								// maximum thickness (% of chord), last 2 digits
		LiftCoeff Cl;										// Stored in radians
		double alphaZeroLift;                               // Stored in radians
		static constexpr double pi = 3.141592653589;
		static constexpr double Cl_2D_SLOPE = 2*pi;			// We will approx that every airfoil has this slope (everyone assumes this)


		// Member Functions
		double calcalphaZeroLift() const;					// returns alphaZeroLift in radians
		// double calcCl0() const;							// Returns Cl0, kinda irrelvant


	};

}

	/*
		NACA
		4 digit number... 
		first digit is max camber in % of chord
		second digit is max chamber in 1/10th chord
		last two digits is max t/c in % chord


		Ex: 2412
		m = Max camber = .02 * chord
		p = Location of max camber = 4/10
		thickness = Max t/c in % chord = .12 (12%)
		

		Can Assum Cl_alpha = 2*pi for m < .05
	*/


#endif