#include <iostream>
#include "to-9km-and-beyond/CF34_3B1.h"
#include "to-9km-and-beyond/TurboFan.h"
using namespace std;

namespace turbofan {

	CF34_3B1::CF34_3B1() : TurboFan::TurboFan() 
	{
	}





	CF34_3B1::CF34_3B1(double inWeight)
		: TurboFan::TurboFan(inWeight) 
	{
	}





	// Copy constrcutor
	CF34_3B1::CF34_3B1(const CF34_3B1& other)
		: TurboFan::TurboFan(other)
	{
		// And then do any specific CF34_3B1 copys
	}


}
