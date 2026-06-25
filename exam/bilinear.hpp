#include "matrix.hpp"
#include <vector>

namespace pp {
	// evaluate the bilinear interpolation of F on the grid x,y at the point (px,py) in the 
	// case where the rectangle (i,j) containing (px,py) is already known
	double bilinear(const std::vector<double>& x, 
			const std::vector<double>& y,
			const Matrix<double>& F,
			double px,
			double py,
			size_t i,
			size_t j);

	// evaluate the bilinear interpolation of F on the grid x,y at the point (px,py)
	double bilinear(const std::vector<double>& x, 
			const std::vector<double>& y,
			const Matrix<double>& F,
			double px,
			double py);

	// evaluate the bilinear interpolation of F on the grid x,y at all points in the grid given by px,py
	Matrix<double> bilinear(const std::vector<double>& x, 
			const std::vector<double>& y,
			const Matrix<double>& F,
			const std::vector<double>& px,
			const std::vector<double>& py);
}
