#include <iostream>
#include "to-9km-and-beyond/kadenMath.h"
#include <vector>
#include <cmath>
using std::vector;


namespace kaden_math {
	double evaluateFunction(const vector<double>& func, double x) {
		// Using Horner's method for efficiency
		double result = 0;
		for (int i = 0; i < func.size(); i++) {
			result = (result * x) + func[i];
		}
		return result;
	}




	vector<double> curveIntersection(const vector<double>& func1, const vector<double>& func2, double xmin, double xmax) {
		int steps = 1000;
		vector<double> intersections;
		double dx = (xmax - xmin) / steps;

		// Logic behind this is to calculate h(x) = f(x)-g(x) and see where that sign changes
		double y1 = evaluateFunction(func1, xmin) - evaluateFunction(func2, xmin);
		for (int i = 0; i < steps; i++) {
			double x2 = ((i + 1) * dx) + xmin;
			double y2 = evaluateFunction(func1, x2) - evaluateFunction(func2, x2);

			if (y1 * y2 <= 0) {
				double intersectPt = ((i * dx) + xmin + x2) / 2.0; // Approxs intersection as halfway between the two points we evaluated
				intersections.push_back(intersectPt);
			}

			y1 = y2;
		}
		return intersections;
	}










	vector<double> curveIntersection(const vector<double>& func1, const vector<double>& func2, double xmin, double xmax, int inSteps) {
		int steps = inSteps;
		vector<double> intersections;
		double dx = (xmax - xmin) / steps;

		// Logic behind this is to calculate h(x) = f(x)-g(x) and see where that sign changes
		double y1 = evaluateFunction(func1, xmin) - evaluateFunction(func2, xmin);
		for (int i = 0; i < steps; i++) {
			double x2 = ((i + 1) * dx) + xmin;
			double y2 = evaluateFunction(func1, x2) - evaluateFunction(func2, x2);

			if (y1 * y2 <= 0) {
				double intersectPt = ((i * dx) + xmin + x2) / 2.0; // Approxs intersection as halfway between the two points we evaluated
				intersections.push_back(intersectPt);
			}

			y1 = y2;
		}
		return intersections;
	}









	// If we run into efficiency problems, we can re-think this implementation
	vector<double> maxDistBetweenFunctions(const vector<double>& func1, const vector<double>& func2, double xmin, double xmax) {
		int steps = 1000;
		double maxDistance = 0;
		double maxLocation = xmin;
		double dx = (xmax - xmin) / steps;

		for (int i = 0; i <= steps; i++) {
			double x = (i * dx) + xmin;

			double fx = evaluateFunction(func1, x);
			double gx = evaluateFunction(func2, x);

			double distance = fabs(fx - gx);

			if (distance > maxDistance) {
				maxLocation = x;
				maxDistance = distance;
			}
		}
		vector<double> result = { maxLocation, maxDistance };
		return result;
	}










	vector<double> maxDistBetweenFunctions(const vector<double>& func1, const vector<double>& func2, double xmin, double xmax, int inSteps) {
		int steps = inSteps;
		double maxDistance = 0;
		double maxLocation = xmin;
		double dx = (xmax - xmin) / steps;

		for (int i = 0; i <= steps; i++) {
			double x = (i * dx) + xmin;

			double fx = evaluateFunction(func1, x);
			double gx = evaluateFunction(func2, x);

			double distance = fabs(fx - gx);

			if (distance > maxDistance) {
				maxLocation = x;
				maxDistance = distance;
			}
		}
		vector<double> result = { maxLocation, maxDistance };
		return result;
	}




}
