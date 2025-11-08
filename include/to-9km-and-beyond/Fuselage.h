#ifndef FUSELAGE_H
#define FUSELAGE_H
// #include

namespace Airplane {
	class Fuselage {
	public:
		Fuselage();
		Fuselage(double inWeight);
		Fuselage(const Fuselage& other); // Copy Constructor
		double getWeight() const;
		void setWeight(double inWeight);


	private:
		// memember variables
		//double area;                 // Probably ignoring it's lift affects
		double weight;
	};




}


#endif
