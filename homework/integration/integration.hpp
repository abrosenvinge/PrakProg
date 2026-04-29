#pragma once
#include <functional>

namespace pp {
	double integrate(const std::function<double(double)>& f, double a, double b, double acc = 0.001, double eps = 0.001);

	double rai(const std::function<double(double)>& f, double a, double b, double acc, double eps, double f2, double f3);
} // namespace pp
