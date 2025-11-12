#ifndef LIFTCOEFF_H
#define LIFTCOEFF_H


namespace airplane {
	class LiftCoeff {
	public:
		LiftCoeff();
		LiftCoeff(double inCL_alpha, double inCL_knott);		// takes in radians
		LiftCoeff(LiftCoeff& other);							// Copy Constructor
		LiftCoeff& operator=(const LiftCoeff& other);			// Assignment Operator

		// Useful Functions:
		double calcLiftCoefficient(double alpha) const;       // Takes in radians


		// Accessors:
		double getCL_Alpha() const;
		double getCL_Knott() const;

		// Mutators:
		void setCL_Alpha(double inTerm);
		void setCL_Knott(double inTerm);

	private:
		double CL_alpha;		// Stored in radians (whatever you put in is the sign)
		double CL_knott;        // (whatever you put in is the sign)
	};

}



#endif