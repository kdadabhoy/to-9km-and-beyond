#ifndef DRAGCOEFF_H
#define DRAGCOEFF_H
#include "to-9km-and-beyond/Wing.h"
#include "to-9km-and-beyond/Fuselage.h"
// Prob should add assignment operator at some point


namespace airplane {
	class DragCoeff {
	public:
		DragCoeff();
		DragCoeff(const airplane::Wing& inWing);
		DragCoeff(const airplane::Fuselage& inFuselage);
		double calcTotalDragCoeff(double AoA, double Reynolds, double Mach, double wetAreaRatio) const;


	private:
		const airplane::Wing* Wing;
		const airplane::Fuselage* Fuselage;
		static constexpr double pi = 3.141592653589;

		double calcParasiteCoeff(double Renyolds, double wetAreaRatio) const;	// For Wing and Fuselage
		double calcInducedCoeff(double AoA) const;								// For Wings only
		double calcCompressibilityCoeff(double Mach) const;						// For Wings only (for now)
		double calcFormDragCoeff(double Cf) const;								// For fuselage only




		// Could re-work class to include 
		// these member variables, but they just take up memory and have no use rn
		
		//double parasiteCoeff;          // Assuming no blunt body drag
		//double inducedCoeff;
		//double compressibilityCoeff;
		//double formDragCoeff;         // Form coefficient for Fuselage only
	};
}



#endif