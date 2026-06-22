#include "bilinear.hpp"
#include <vector>
#include "matrix.hpp"
#include "splines.hpp"
#include <iostream>

namespace pp {
	double bilinear(const std::vector<double>& x, 
			const std::vector<double>& y,
			const Matrix<double>& F,
			double px,
			double py)
	{
		if (px < x[0] || px > x.back()) std::cerr << "px\n";
		if (py < y[0] || py > y.back()) std::cerr << "py\n";
		// std::cerr << px << " " << py << "\n";
		size_t i = binary_search(x, px);
		size_t j = binary_search(y, py);
		std::cerr << std::format("{} {} {} {}\n", px, i, py, j);

		double dx = x[i+1] - x[i];
		double dy = y[j+1] - y[j];

		double a = F[i,j];
		double b = (F[i+1,j] - a)/dx;
		double c = (F[i,j+1] - a)/dy;
		double d = (F[i+1,j+1] - a - b*dx - c*dy) / (dx*dy);

		return a + b*(px-x[i]) + c*(py-y[j]) + d*(px-x[i])*(py-y[j]);
	}
}
