#include "bilinear.hpp"
#include <vector>
#include "matrix.hpp"
#include "splines.hpp"

namespace pp {
	// evaluate the bilinear interpolation of F on the grid x,y at the point (px,py) in the 
	// case where the rectangle (i,j) containing (px,py) is already known
	double bilinear(const std::vector<double>& x, 
			const std::vector<double>& y,
			const Matrix<double>& F,
			double px,
			double py,
			size_t i,
			size_t j) 
	{
		double dx = x[i+1] - x[i];
		double dy = y[j+1] - y[j];

		double a = F[i,j];
		double b = (F[i+1,j] - a)/dx;
		double c = (F[i,j+1] - a)/dy;
		double d = (F[i+1,j+1] - a - b*dx - c*dy) / (dx*dy);

		return a + b*(px-x[i]) + c*(py-y[j]) + d*(px-x[i])*(py-y[j]);
	}

	double bilinear(const std::vector<double>& x, 
			const std::vector<double>& y,
			const Matrix<double>& F,
			double px,
			double py)
	{
		if (px < x[0] || px > x.back()) throw std::out_of_range(std::format("px = {} not between first and last element of x ({}, {})", px, x[0], x.back()));
		if (py < y[0] || py > y.back()) throw std::out_of_range("px not between first and last element of x");
		size_t i = binary_search(x, px);
		size_t j = binary_search(y, py);
		
		return bilinear(x,y,F,px,py,i,j);
	}

	Matrix<double> bilinear(const std::vector<double>& x, 
			const std::vector<double>& y,
			const Matrix<double>& F,
			const std::vector<double>& px,
			const std::vector<double>& py) 
	{
		Matrix<double> out(px.size(), py.size());
		size_t j = 0;
		for (size_t pj = 0; pj < py.size(); ++pj) {
			double pyj = py[pj];
			while (pyj > y[j+1]) j++;
			size_t i = 0;
			for (size_t pi = 0; pi < px.size(); ++pi) {
				double pxi = px[pi];
				while (pxi > x[i+1]) i++;

				out[pi,pj] = bilinear(x,y,F,pxi,pyj,i,j);
			}
		}
		return out;
	}
}
