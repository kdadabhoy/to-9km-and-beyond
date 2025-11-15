#ifndef KADENMATH
#define KADENMATH
#include <fstream>
#include <vector>
#include <string>
using std::vector;
using std::string;
// Kaden's own custom math functions

/*
	
	// Polynomial Math:
		Functions are passed in by using vectors:
			The length of the vector determines the highest term.
			Ex: 4x^3 + 3x^2 + x + 10 would be in a vector <4, 3, 0, 1, 10>	



	evalulateFunction takes in one functions and the x coord to eval it at
	

	curveIntersection takes in two functions in the for of vectors. 
		ASSUMES 1000 steps if you use the form where you don't specify
		The function also takes in the domain (xmin and xmax) that you want to check intersections for.
		The function then returns the x-cords where the curves intersect
		The function returns an empty vector if there is no intersections


	maxDistBetweenFunctions takes in two functions in the for of vectors. 
		ASSUMES 1000 steps if you use the form where you don't specify
		The function also takes in the domain (xmin and xmax)
		The function then returns a vector with the first element being the x-cord of maxDistance
		and the second element being the maxDistance.



	// Vector Math and Vector-Based Functions:

	maxDistBetweenCurves takes in 3 data sets (x, y1, y2). It then returns a vector = {maxLocation, maxDistance} of y1 and y2
	saveVectorsToCSV takes in 2 or 3 vectors (x,y1,y2) of the same size and converts the data to a CSV file with the filename




*/


namespace kaden_math {
	// Polynomial Function Math
	double         evaluateFunction(const vector<double>& func, double x);
	vector<double> curveIntersection(const vector<double>& func1, const vector<double>& func2, double xmin, double xmax);
	vector<double> curveIntersection(const vector<double>& func1, const vector<double>& func2, double xmin, double xmax, int inSteps);
	vector<double> maxDistBetweenFunctions(const vector<double>& func1, const vector<double>& func2, double xmin, double xmax);
	vector<double> maxDistBetweenFunctions(const vector<double>& func1, const vector<double>& func2, double xmin, double xmax, int inSteps);


	// Vector Math and Vector Based-Functions
	vector<double> maxDistBetweenCurves(const vector<double>& xdata, const vector<double>& y1data, const vector<double>& y2data);

	void saveVectorsToCSV(const vector<double>& x, const vector<double>& y, const string& filename);
	void saveVectorsToCSV(const vector<double>& x, const vector<double>& y1, const vector<double>& y2, const string& filename);

}


#endif