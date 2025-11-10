#include <iostream>
#include "to-9km-and-beyond/TurboFan.h"
using namespace std;


namespace airplane {

	TurboFan::TurboFan() {
		weight = 0;
		SSLThrust = 0;
		specificFuelConsumption = 0;
		thrustPerSSLThrust = 0;
	}




	TurboFan::TurboFan(double inWeight, double inSSLThrust) {
		weight = inWeight;
		SSLThrust = inSSLThrust;
		specificFuelConsumption = .5;  // should prob take input
		thrustPerSSLThrust = 1;        // should prob take input
	}





	// Copy constrcutor
	TurboFan::TurboFan(const TurboFan& other) {
		weight = other.weight;
		SSLThrust = other.SSLThrust;
		specificFuelConsumption = other.specificFuelConsumption;
		thrustPerSSLThrust = other.thrustPerSSLThrust;
	}




	// Accessors:
	double TurboFan::getWeight() const {
		return weight;
	}

	double TurboFan::getSSLThrust() const {
		return SSLThrust;
	}

	double TurboFan::getThrustPerSSLThrust() const {
		return thrustPerSSLThrust;
	}

	double TurboFan::getSpecificFuelConsumption() const {
		return specificFuelConsumption;
	}






	// Mutators:
	void TurboFan::setWeight(double inWeight) {
		weight = inWeight;
	}


	void TurboFan::setSSLThrust(double inThrust) {
		SSLThrust = inThrust;
	}


	void TurboFan::setThrustPerSSLThrust(double inFrac) {
		thrustPerSSLThrust = inFrac;
	}


	void TurboFan::setSpecificFuelConsumption(double inFrac) {
		specificFuelConsumption = inFrac;
	}



	// Member Function
	double TurboFan::getThrust() const {
		return thrustPerSSLThrust * SSLThrust;
	}
}
