#pragma once
#include <functional>
#include <tuple>

namespace pp {
	std::tuple<double,double> integrate(const std::function<double(double)>& f, 
			double a,
			double b,
			double acc = 0.0001,
			double eps = 0.0001);

	std::tuple<double,double> rai(const std::function<double(double)>& f,
			double a,
			double b,
			double acc,
			double eps,
			double f2,
			double f3);

	std::tuple<double,double> Clenshaw_Curtis_quad(const std::function<double(double)>& f, 
			double a, 
			double b, 
			double acc = 0.0001, 
			double eps = 0.0001);

	std::tuple<std::function<double(double)>,double,double> transform_inf_interval(const std::function<double(double)>& f,
			double a,
			double b);

	double erf(double z, double acc = 0.001, double eps = 0.001);
} // namespace pp
