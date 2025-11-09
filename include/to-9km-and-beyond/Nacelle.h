#ifndef NACELLE_H
#define NACELLE_H
// #include

namespace airplane {
	class Nacelle {
	public:
		Nacelle();
		Nacelle(double inWeight);
		Nacelle(const Nacelle& other); // Copy Constructor
		double getWeight() const;
		void setWeight(double inWeight);

	private:
		double weight;
	};




}


#endif