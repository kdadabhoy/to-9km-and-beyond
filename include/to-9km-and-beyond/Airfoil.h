#ifndef AIRFOIL_H
#define AIRFOIL_H
#include "to-9km-and-beyond/LiftCoeff.h"
#include <string>
// Intended to work with 4 series NACA airfoils
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
		Airfoil(const std::string& inNACA, const LiftCoeff& inCl, double inAlphaZeroLift);
		double getCl_deg(double inAlphaDeg);
		double getCl_rad(double inAlphaRad);

		double getCl_AlphaTerm();				// Radians
		double getCl_alphaZeroLift();			// Radians




	private:
		std::string NACA;               
		double maxCamber;									// Maximum camber (% of chord), 1st digit
		double posMaxCamber;								// Location of maximum camber (% of chord), 2nd digit
		double maxThickness;								// maximum thickness (% of chord), last 2 digits
		LiftCoeff Cl;										// Stored in radians
		double alphaZeroLift;                               // Stored in radians
		static constexpr double pi = 3.141592653589;


		// Member Functions
		LiftCoeff calcCl() const;							// Returns Lift Coeff
		double calcalphaZeroLift() const;							// returns alphaZeroLift in radians

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