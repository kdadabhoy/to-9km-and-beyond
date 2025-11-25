#ifndef DRAGCOEFF_H
#define DRAGCOEFF_H
#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/Fuselage.h"
// Might want to rework this class a bit for future projects

namespace airplane {
	class DragCoeff {
	public:
	// Constructors
		DragCoeff();
		DragCoeff(const airplane::Wing& inWing);
		DragCoeff(const airplane::Fuselage& inFuselage);


	// Main Function
		double calcTotalDragCoeff(double AoA, double Reynolds, double Mach, double wetAreaRatio) const;


	// Useful Functions:
		double calcParasiteCoeff(double Renyolds, double wetAreaRatio) const;	// For Wing and Fuselage
		double calcInducedCoeff(double AoA) const;								// For Wings only
		double calcCompressibilityCoeff(double Mach, double AoA) const;			// For Wings only (for now)
		double calcFormDragCoeff(double Cf) const;								// For fuselage only


	private:
		const airplane::Wing* Wing;
		const airplane::Fuselage* Fuselage;


	// Constants:
		static constexpr double pi = 3.141592653589;
		static constexpr double CRITICAL_REYNOLDS_NUMBER = 5e5;                  

	};
}



#endif














/*
	Notes:
		// Could re-work class to include 
		// these member variables, but they just take up memory and have no use rn
		
		//double parasiteCoeff;          // Assuming no blunt body drag
		//double inducedCoeff;
		//double compressibilityCoeff;
		//double formDragCoeff;         // Form coefficient for Fuselage only





*/