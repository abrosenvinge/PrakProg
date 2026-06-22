#include "matrix.hpp"
#include <vector>

namespace pp {
	double bilinear(const std::vector<double>& x, 
			const std::vector<double>& y,
			const Matrix<double>& F,
			double px,
			double py);
}
