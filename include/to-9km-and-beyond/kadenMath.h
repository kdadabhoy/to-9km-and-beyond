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
	

	functionIntersection takes in two functions in the for of vectors. 
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

	curveIntersection takes in takes in 3 data sets (x, y1, y2).
		- y1 and y2 must have the same x scale and be the same size
		- This function will compare every single data point to see if there is an intersection
			- If there is an intersection, it estimates the intersection x coord (midpoint of them)
		- The function then returns a vector of x-cords where the curves intersect
		- The function returns an empty vector if there is no intersections



	curveIntersection takes in 3 data sets and how often to check (x, y1, y2, skip).
		- y1 and y2 must have the same x scale and be the same size
		- skip = 2 then we compare every other point to see if there is an intersection
			- skip = 3 then every third point... etc... lose accuracy higher skip is
		- The function then returns a vector of x-cords where the curves intersect
		- The function returns an empty vector if there is no intersections



	maxDistBetweenCurves takes in 3 data sets (x, y1, y2).
		- It does not care if the curves intersect or which curve is on top.
		- It then returns a vector = {maxLocation, maxDistance} of y1 and y2




	maxDistBetweenCurves2 takes in 3 data sets (x, y1, y2). 
		- It is sensitive to y1 and y2. ONLY RECORDS DISTANCES WHEN y1>y2
		- Returns an empty vector if y1 is never greater than y2
		- It then returns a vector = {maxLocation, maxDistance} of y1 and y2

	saveVectorsToCSV takes in 2 or 3 vectors (x,y1,y2) of the same size and converts the data to a CSV file with the filename




*/


namespace kaden_math {
	// Polynomial Function Math
	double         evaluateFunction(const vector<double>& func, double x);
	vector<double> functionIntersection(const vector<double>& func1, const vector<double>& func2, double xmin, double xmax);
	vector<double> functionIntersection(const vector<double>& func1, const vector<double>& func2, double xmin, double xmax, int inSteps);
	vector<double> maxDistBetweenFunctions(const vector<double>& func1, const vector<double>& func2, double xmin, double xmax);
	vector<double> maxDistBetweenFunctions(const vector<double>& func1, const vector<double>& func2, double xmin, double xmax, int inSteps);


	// Vector Math and Vector Based-Functions
	vector<double> curveIntersection(const vector<double>& x, const vector<double>& y1, const vector<double>& y2);
	vector<double> curveIntersection(const vector<double>& x, const vector<double>& y1, const vector<double>& y2, double skip);
	vector<double> maxDistBetweenCurves(const vector<double>& xdata, const vector<double>& y1data, const vector<double>& y2data);
	vector<double> maxDistBetweenCurves2(const vector<double>& xdata, const vector<double>& y1data, const vector<double>& y2data);

	void saveVectorsToCSV(const vector<double>& x, const vector<double>& y, const string& filename);
	void saveVectorsToCSV(const vector<double>& x, const vector<double>& y1, const vector<double>& y2, const string& filename);


	// Linear Math
	double linearInterpolate(double point, double x1, double y1, double x2, double y2);

}


#endif