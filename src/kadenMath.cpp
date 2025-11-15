#include <iostream>
#include "to-9km-and-beyond/kadenMath.h"
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <string>
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::fabs;


namespace kaden_math {

// Polynomial Math:

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
		return { maxLocation, maxDistance };
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
		return { maxLocation, maxDistance };
	}
	








// Vector Math and Functions


	vector<double> maxDistBetweenCurves(const vector<double>& xdata, const vector<double>& y1data, const vector<double>& y2data) {

		if (y1data.size() != y2data.size() || xdata.size() != y1data.size()) {
			cout << "Error: Data set sizes do no match" << endl;
			return {};
		} 

		double maxLocation = xdata[0];
		double maxDistance = fabs(y1data[0] - y2data[0]);

		for (int i = 0; i < xdata.size(); i++) {
			double difference = fabs(y1data[i] - y2data[i]);
			if (difference > maxDistance) {
				maxLocation = xdata[i];
				maxDistance = difference;
			}
		}
		return { maxLocation, maxDistance };
	}












	void saveVectorsToCSV(const vector<double>& x, const vector<double>& y, const string& filename) {

		if (x.size() != y.size()) {
			cout << "Error: Vecs not the same size" << endl;
			return;
		}

		std::ofstream file(filename);
		if (!file.is_open()) {
			cout << "Could not open file" << endl;
			return;
		}

		file << std::fixed << std::setprecision(6);

		for (int i = 0; i < x.size(); i++) {
			file << x[i] << ", " << y[i] << "\n";
		}

		file.close();
		cout << "Data is saved to: " << filename << endl;
		return;
	}











	void saveVectorsToCSV(const vector<double>& x, const vector<double>& y1, const vector<double>& y2, const string& filename) {

		if (x.size() != y1.size() && x.size() != y2.size()) {
			cout << "Error: Vecs not the same size" << endl;
			return;
		}

		std::ofstream file(filename);
		if (!file.is_open()) {
			cout << "Could not open file" << endl;
			return;
		}


		file << std::fixed << std::setprecision(6);

		for (int i = 0; i < x.size(); i++) {
			file << x[i] << ", " << y1[i] << ", " << y2[i] << "\n";
		}

		file.close();
		cout << "Data is saved to: " << filename << endl;
		return;
	}


}
