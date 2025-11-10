#ifndef CF34_3B1_H
#define CF34_3B1_H
#include "to-9km-and-beyond/TurboFan.h"


namespace airplane {
	class CF34_3B1 : public TurboFan {
	public:
		CF34_3B1();												// Call this one, already has normal characteristics
		CF34_3B1(double inWeight, double inStaticThrust);       // If you ever wanted to have this enginge.. but different weight and staticThrust
		CF34_3B1(const CF34_3B1& other);						// Copy Constructor

	private:

	};


}


#endif