#ifndef CF34_3B1_H
#define CF34_3B1_H
#include "to-9km-and-beyond/TurboFan.h"


namespace airplane {
	class CF34_3B1 : public TurboFan {
	public:
		CF34_3B1();
		CF34_3B1(double inWeight, double inStaticThrust);
		CF34_3B1(const CF34_3B1& other); // Copy Constructor

	private:


	};


}


#endif