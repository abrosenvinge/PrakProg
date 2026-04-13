#pragma once
// #include <array>
#include <functional>
#include <tuple>
#include <vector>
#include "../lib/matrix.hpp"

namespace pp {

	// class StepperRK45 {
	// private:
	// 	using FUNC = const std::function<void(double, const std::vector<double>&)>&;
	// 	std::array<double, 5> k;
	// 	std::vector<double> y;
	// 	FUNC f;
	// public:
	// 	StepperRK45(FUNC f, std::vector<double> y0) : f(f), y(y0) {}
	//
	//
	// };

	std::tuple<Vector<double>, Vector<double>> rkstep45(
			const std::function<Vector<double>(double, const Vector<double>&)> f,
			double x,
			const Vector<double>& y,
			double h
	);

	std::tuple<std::vector<double>, std::vector<double>> driver(
			std::function<Vector<double>(double, const Vector<double>&)> f,
			double a,
			double b,
			Vector<double> y0,
			double h = 0.125,
			double acc = 0.01,
			double eps = 0.01
	);
}
