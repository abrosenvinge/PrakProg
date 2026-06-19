#pragma once
// #include <array>
#include <functional>
#include <limits>
#include <tuple>
#include <vector>
#include "../lib/matrix.hpp"

namespace pp {
	std::tuple<Vector<double>, Vector<double>> rkstep45(
			const std::function<Vector<double>(double, const Vector<double>&)>& f,
			double x,
			const Vector<double>& y,
			double h
	);

	std::tuple<std::vector<double>, std::vector<Vector<double>>> driver(
			const std::function<Vector<double>(double, const Vector<double>&)>& f,
			double a,
			double b,
			const VectorBase<double>& y0,
			double h = 0.125,
			double acc = 0.01,
			double eps = 0.01,
			double max_step = std::numeric_limits<double>::infinity()
	);
	
	// same as above, but only return f(b)
	Vector<double> endpoint_driver(
			const std::function<Vector<double>(double, const Vector<double>&)>& f,
			double a,
			double b,
			const VectorBase<double>& y0,
			double h = 0.125,
			double acc = 0.01,
			double eps = 0.01,
			double max_step = std::numeric_limits<double>::infinity()
	);
}
