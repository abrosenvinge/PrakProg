#include <algorithm>
#include <cmath>
#include "approx.hpp"
namespace approx {
	bool approx(double a, double b, double acc, double eps) {
		double diff = std::abs(a - b);
		return diff <= acc || diff <= eps * std::max(std::abs(a), std::abs(b));
	}
}
