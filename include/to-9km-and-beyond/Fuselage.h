#ifndef FUSELAGE_H
#define FUSELAGE_H
// #include

namespace airplane {
	class Fuselage {
	public:
		Fuselage();
		Fuselage(double inWeight);
		Fuselage(const Fuselage& other); // Copy Constructor
		Fuselage& operator=(const Fuselage& other);
		double getWeight() const;
		void setWeight(double inWeight);


	private:
		// memember variables
		//double area;                 // Probably ignoring it's lift affects
		double weight;
	};




}


#endif
