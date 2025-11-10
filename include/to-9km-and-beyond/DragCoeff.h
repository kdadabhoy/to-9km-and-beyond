#ifndef DRAGCOEFF_H
#define DRAGCOEFF_H
#include "to-9km-and-beyond/Wing.h"
// Prob should add assignment operator at some point


namespace airplane {
	class DragCoeff {
	public:
		DragCoeff();
		DragCoeff(airplane::Wing& inWing, double inReferenceArea);

		double calcTotalDragCoeff(double AoA, double velocity, double kinematicViscosity, double temp, double wetAreaRatio) const;
		double calcTotalDragCoeff(double AoA, double Reynolds, double Mach) const;
		double calcTotalDragCoeff(double AoA, double Reynolds, double Mach, double wetAreaRatio) const;


	private:
		double parasiteCoeff;          // Assuming no blunt body drag
		double inducedCoeff;
		double compressibilityCoeff;

		airplane::Wing* Wing;
		double referenceArea;
		static constexpr double pi = 3.141592653589;
		static constexpr double GAS_CONSTANT = 1716;


		double calcParasiteCoeff(double velocity, double kinematicViscosity, double wetAreaRatio) const;
		double calcParasiteCoeff(double Renyolds) const;
		double calcParasiteCoeff(double Renyolds, double wetAreaRatio) const;


		double calcInducedCoeff(double AoA) const;
		double calcCompressibilityCoeff(double Mach) const;
	};
}



#endif