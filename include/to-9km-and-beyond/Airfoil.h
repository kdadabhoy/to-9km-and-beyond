#ifndef AIRFOIL_H
#define AIRFOIL_H


namespace Airplane {
	class Airfoil {
	public:
		//Airfoil();
		//Airfoil(int inNACA);


	private:
		int NACA;
		double thickness;
		double chord;
		// need a function to calculate lift coeffs
		// need function to convert to 3D lift
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