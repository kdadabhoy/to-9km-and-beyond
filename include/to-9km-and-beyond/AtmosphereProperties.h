#ifndef ATMOSPHEREPROPERTIES_H
#define ATMOSPHEREPROPERTIES_H

// Class operates in Imperial Units (and Returns Imperial Units)
// TLDR: Implementation of NASA Equations & a few others to get atmopshere properties




namespace atmosphere_properties {
	class AtmosphereProperties {
	public:
		AtmosphereProperties();
		AtmosphereProperties(double inHeight);


	//Mutator:
		void setHeight(double inHeight);            // Sets new height and recalculates all propertites


	//Accessors:
		double getDensity() const;                  // slug/ft^3
		double getViscosity() const;                // slug/(ft*s)
		double getKinematicVisc() const;            // ft^2/s
		double getStaticPressure() const;           // lb/ft^2 
		double getTemperature() const;              // R
		double getSpeedOfSound() const;             // ft/s
		double getHeight() const;                   // ft



	// Other functions - Suggested to just use setHeight and accessors (more efficient)
		double calcTemperature(double inHeight) const;        // Rankine      Using NASA Model (sea level -> h=0)
		double calcDensity(double inHeight) const;            // slug/ft^3,   Using P=rho*R*T
		double calcStaticPressure(double inHeight) const;     // lb/ft^2      Using NASA Model(sea level -> h=0)
		double calcViscosity(double inHeight) const;          // slug/(ft*s)  Using Sutherland Law
		double calcKinematicVisc(double inHeight) const;      // ft^2/s,      Using v = mu / rho
		double calcSpeedofSound(double inHeight) const;       // ft/s



	private:
		double height;
		double density;               // slug / ft^3
		double staticPressure;        // lbf/sqft
		double viscosity;             // slug/(ft*s)
		double kinematicViscosity;    // ft^2/s
		double temperature;           // Rankine
		double speedOfSound;          // ft/s


	//Useful Constants
		static const int TROPOSPHERE_UPPER_LIMIT = 36152;               // NASA Model
		static const int LOWER_STRATOSPHERE_UPPER_LIMIT = 82345;        // NASA Model
		static constexpr double F_to_R = 459.7;                         // add this for deg F to deg R
		static const int GAS_CONSTANT = 1718;                           // ft*lbf/(slug*R) 
		static constexpr double SUTHERLAND_REF_VISC = 3.737e-7;         // slug/(ft*s)
		static constexpr double SUTHERLAND_REF_TEMP = 518.67;           // Rankine
		static constexpr double SUTHERLAND_CONSTANT= 198.72;            // Rankine
		static constexpr double GAMMA = 1.4;                            // Air gamma


	// calcAndSet functions:
		void calcAndSetAllPropertities();    // Calcs all propertites in an effiecient manner.
		void calcAndSetTemperature();		 // Rankine         Using NASA Model (sea level -> h=0)
		void calcAndSetDensity();            // slug/ft^3,      Using P=rho*R*T
		void calcAndSetStaticPressure();     // lb/ft^2         Using NASA Model (sea level -> h=0)
		void calcAndSetViscosity();			 // slug/(ft*s)     Using Sutherland Law
		void calcAndSetKinematicVisc();      // ft^2/s,         Using v = mu / rho
		void calcAndSetSpeedofSound();       // ft/s
	};
}


#endif