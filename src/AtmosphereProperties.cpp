#include <iostream>
#include "to-9km-and-beyond/AtmosphereProperties.h"
#include <cmath>
#include <cassert>
using namespace std;
// Prob should add assignment operator at some point



namespace atmosphere_properties {
	AtmosphereProperties::AtmosphereProperties() {
		height = 0;
		calcAndSetAllPropertities();
	}








	AtmosphereProperties::AtmosphereProperties(double inHeight) {
		height = inHeight;
		calcAndSetAllPropertities();
	}








// Calcs and sets all properties based on height member variable
	void AtmosphereProperties::calcAndSetAllPropertities() {
		calcAndSetTemperature();			// Requires height
		calcAndSetStaticPressure();         // Requires temperature
		calcAndSetDensity();                // Requires temperature
		calcAndSetViscosity();              // Requires temperature
		calcAndSetKinematicVisc();          // Requires viscosity and density
		calcAndSetSpeedofSound();           // Requires temperature
	}








// calcAndSet Functions:

	void AtmosphereProperties::calcAndSetTemperature() {
		assert(height >= 0);

		if (height < TROPOSPHERE_UPPER_LIMIT) {
			temperature = (59 - (.00356 * height)) + F_to_R;
			return;
		} else if (height < LOWER_STRATOSPHERE_UPPER_LIMIT) {
			temperature = -70 + F_to_R;
			return;
		} else {
			temperature = (-205.05 + (.00165 * height)) + F_to_R;
			return;
		}
	}








	void AtmosphereProperties::calcAndSetStaticPressure() {
		assert(height >= 0);

		if (height < TROPOSPHERE_UPPER_LIMIT) {
			staticPressure = 2116 * pow((temperature / 518.6), 5.256);
			return;
		} else if (height < LOWER_STRATOSPHERE_UPPER_LIMIT) {
			staticPressure = 473.1 * exp(1.73 - (.000048 * height));
			return;
		} else {
			staticPressure = 51.97 * pow((temperature / 389.98), -11.388);
			return;
		}
	}








	void AtmosphereProperties::calcAndSetDensity() {
		density = staticPressure / (GAS_CONSTANT * temperature);
		return;
	}








	void AtmosphereProperties::calcAndSetViscosity() {
		viscosity = SUTHERLAND_REF_VISC * pow((temperature / SUTHERLAND_REF_TEMP), 1.5)
			* ((SUTHERLAND_REF_TEMP + SUTHERLAND_CONSTANT) / (temperature + SUTHERLAND_CONSTANT));
		return;
	}








	void AtmosphereProperties::calcAndSetKinematicVisc() {
		kinematicViscosity = viscosity / density;
		return;
	}








	void AtmosphereProperties::calcAndSetSpeedofSound() {
		speedOfSound = sqrt(GAMMA * GAS_CONSTANT * temperature);
		return;
	}










// calc Public Functions:

	double AtmosphereProperties::calcTemperature(double inHeight) const {
		assert(inHeight >= 0);

		if (inHeight < TROPOSPHERE_UPPER_LIMIT) {
			return (59 - (.00356 * inHeight)) + F_to_R;
		} else if (inHeight < LOWER_STRATOSPHERE_UPPER_LIMIT) {
			return -70 + F_to_R;
		} else {
			return (-205.05 + (.00165 * inHeight)) + F_to_R;
		}
	}








	double AtmosphereProperties::calcStaticPressure(double inHeight) const{
		assert(inHeight >= 0);
		double temp = calcTemperature(inHeight);

		if (inHeight < TROPOSPHERE_UPPER_LIMIT) {
			return 2116 * pow((temp / 518.6), 5.256);
		} else if (inHeight < LOWER_STRATOSPHERE_UPPER_LIMIT) {
			return 473.1 * exp(1.73 - (.000048 * inHeight));
		} else {
			return 51.97 * pow((temp / 389.98), -11.388);
		}
	}








	double AtmosphereProperties::calcDensity(double inHeight) const {
		double pressure = calcStaticPressure(inHeight);
		double temperature = calcTemperature(inHeight);
		return pressure / (GAS_CONSTANT * temperature);

	}








	double AtmosphereProperties::calcViscosity(double inHeight) const {
		double temp = calcTemperature(inHeight);
		double visc;
		visc = SUTHERLAND_REF_VISC* pow((temp / SUTHERLAND_REF_TEMP), 1.5) 
			* ((SUTHERLAND_REF_TEMP + SUTHERLAND_CONSTANT) / (temp + SUTHERLAND_CONSTANT));
		return visc;
	}








	double AtmosphereProperties::calcKinematicVisc(double inHeight) const {
		double density = calcDensity(inHeight);
		double visc = calcViscosity(inHeight);
		return visc / density;
	}








	double AtmosphereProperties::calcSpeedofSound(double inHeight) const {
		double temp = calcTemperature(inHeight);
		return sqrt(GAMMA * GAS_CONSTANT * temp);
	}










// Mutators:

	void AtmosphereProperties::setHeight(double inHeight) {
		height = inHeight;
		calcAndSetAllPropertities();
	}










// Accessors:

	double AtmosphereProperties::getDensity() const {
		return density;
	}








	double AtmosphereProperties::getViscosity() const {
		return viscosity;
	}








	double AtmosphereProperties::getKinematicVisc() const {
		return kinematicViscosity;
	}








	double AtmosphereProperties::getStaticPressure() const {
		return staticPressure;
	}








	double AtmosphereProperties::getTemperature() const {
		return temperature;
	}








	double AtmosphereProperties::getSpeedOfSound() const {
		return speedOfSound;
	}








	double AtmosphereProperties::getHeight() const {
		return height;
	}








}