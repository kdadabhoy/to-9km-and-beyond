#ifndef DRAGCOEFF_H
#define DRAGCOEFF_H
#include "to-9km-and-beyond/Wing.h"


namespace AeroCoeff {
	class DragCoeff {
	public:
		DragCoeff();
		DragCoeff(const Airplane::Wing& inWing, const double inReferenceArea);
		double calcTotalDragCoeff(const double AoA, const double velocity,
			const double kinematicViscosity, const double temp);

	private:
		double parasiteCoeff;          // Assuming no blunt body drag
		double inducedCoeff;
		double compressibCoeff;

		Airplane::Wing Wing;
		double referenceArea;

		double calcParasiteCoeff(const double velocity, const double kinematicViscosity);
		double calcInducedCoeff(const double AoA);
		double calcCompressibCoeff(const double Mach);
	};
}



#endif