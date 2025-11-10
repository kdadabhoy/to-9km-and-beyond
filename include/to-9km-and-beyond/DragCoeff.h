#ifndef DRAGCOEFF_H
#define DRAGCOEFF_H
#include "to-9km-and-beyond/Wing.h"
// Prob should add assignment operator at some point


namespace aeroCoeff {
	class DragCoeff {
	public:
		DragCoeff();
		DragCoeff(const airplane::Wing& inWing, const double inReferenceArea);
		double calcTotalDragCoeff(const double AoA, const double velocity,
			const double kinematicViscosity, const double temp);

	private:
		double parasiteCoeff;          // Assuming no blunt body drag
		double inducedCoeff;
		double compressibCoeff;

		const airplane::Wing* Wing;
		double referenceArea;
		static constexpr double pi = 3.141592653589;

		double calcParasiteCoeff(const double velocity, const double kinematicViscosity);
		double calcInducedCoeff(const double AoA);
		double calcCompressibCoeff(const double Mach);
	};
}



#endif