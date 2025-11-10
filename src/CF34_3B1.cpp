#include <iostream>
#include "to-9km-and-beyond/CF34_3B1.h"
#include "to-9km-and-beyond/TurboFan.h"
using namespace std;
// Prob should add assignment operator at some point



namespace airplane {

	CF34_3B1::CF34_3B1() : TurboFan::TurboFan(1670, 19500)
	{
		// Assumes weight of CF34_3B1 is 1670 lbm
		// Assumes Standard Day Sea Level Static Thrust of CF34_3B1 is 19500 lb	
	}





	CF34_3B1::CF34_3B1(double inWeight, double inStaticThrust)
		: TurboFan::TurboFan(inWeight, inStaticThrust) 
	{
	}





	// Copy constrcutor
	CF34_3B1::CF34_3B1(const CF34_3B1& other)
		: TurboFan::TurboFan(other)
	{
		// And then do any specific CF34_3B1 copys
	}




}
