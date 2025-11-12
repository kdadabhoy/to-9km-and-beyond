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


		double calcTotalDragCoeff(double AoA, double Reynolds, double Mach, double wetAreaRatio) const;			// Use this one 9/10 times
		// double calcTotalDragCoeff(double AoA, double velocity, double kinematicViscosity, double temp, double wetAreaRatio) const;



	private:
		const airplane::Wing* Wing;
		const airplane::Fuselage* Fuselage;

		static constexpr double pi = 3.141592653589;
		static constexpr double GAS_CONSTANT = 1716;

		//double parasiteCoeff;          // Assuming no blunt body drag
		//double inducedCoeff;
		//double compressibilityCoeff;
		//double formDragCoeff;         // Form coefficient for Fuselage only



		double calcParasiteCoeff(double Renyolds, double wetAreaRatio) const;
		//double calcParasiteCoeff(double velocity, double kinematicViscosity, double wetAreaRatio) const;


		double calcInducedCoeff(double AoA) const;
		double calcCompressibilityCoeff(double Mach) const;

		// Fuselage only
		double calcFormDragCoeff(double Cf) const;

	};
}



#endif