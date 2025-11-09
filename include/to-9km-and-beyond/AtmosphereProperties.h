#ifndef ATMOSPHEREPROPERTIES_H
#define ATMOSPHEREPROPERTIES_H
// #include

namespace AtmosphereProperties {
	class AtmosphereProperties {
	public:
		AtmosphereProperties();
		AtmosphereProperties(double inHeight);

		//Mutator:
		void setHeight(double inHeight);                    // Sets new height and recalculates all propertites

		//Accessors:
		double getDensity() const;
		double getViscosity() const;
		double getKinematicVisc() const;
		double getStaticPressure() const;
		double getTemperature() const;

		// Other functions - Suggested to just use setHeight and accessors
		// Especially if you need all quantities (for efficiency reasons)
		double calcTemperature(double inHeight) const;        // Rankine         Using NASA Model
		double calcDensity(double inHeight) const;            // slug/ft^3,      Using P=rho*R*T
		double calcStaticPressure(double inHeight) const;     // lb/ft^2       Using NASA Model
		double calcViscosity(double inHeight) const;          // slug/(ft*s)     Using Sutherland Law
		double calcKinematicVisc(double inHeight) const;      // ft^2/s,       Using v = mu / rho



	private:
		double height;
		double density;               // slug / ft^3
		double staticPressure;        // lbf/sqft
		double viscosity;             // slug/(ft*s)
		double kinematicViscosity;    // ft^2/s
		double temperature;           // Rankine

		//Useful Constants
		static const int TROPOSPHERE_UPPER_LIMIT = 36152;               // NASA Model
		static const int LOWER_STRATOSPHERE_UPPER_LIMIT = 36152;        // NASA Model
		static const int F_to_R = 459.7;                                // add this for deg F to deg R
		static const int GAS_CONSTANT = 1718;                           // ft*lbf/(slug*R) 
		static const int SUTHERLAND_REF_VISC = 3.737e-7;                // slug/(ft*s)
		static const int SUTHERLAND_REF_TEMP = 518.67;                  // Rankine
		static const int SUTHERLAND_CONSTANT= 198.72;                   // Rankine


		void calcAllPropertities();    // Calcs all propertites in an effiecient manner.


		// These functions calculate and set their respective member variable for the height that is stored.
		void calcTemperature();        // Rankine         Using NASA Model
		void calcDensity();            // slug/ft^3,      Using P=rho*R*T
		void calcStaticPressure();     // lb/ft^2       Using NASA Model
		void calcViscosity();          // slug/(ft*s)     Using Sutherland Law
		void calcKinematicVisc();      // ft^2/s,       Using v = mu / rho

	};
}


#endif