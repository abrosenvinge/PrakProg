#include "integration.hpp"
#include <cmath>
#include <cstdlib>
#include <functional>
#include <numbers>
#include <tuple>

namespace pp {
	std::tuple<double,double> integrate(const std::function<double(double)>& f, double a, double b, double acc, double eps) {
		auto [f_,a_,b_] = transform_inf_interval(f, a, b);
		double h = b_ - a_;
		double f2 = f_(a_ + 2*h / 6.);
		double f3 = f_(a_ + 4*h / 6.);

		return rai(f_, a_, b_, acc, eps, f2, f3);
	}

	std::tuple<double,double> rai(const std::function<double(double)>& f, double a, double b, double acc, double eps, double f2, double f3) {
		double h = b-a;

		double f1 = f(a + h / 6.), f4 = f(a + 5*h / 6.);

		double Q = (2*f1 + f2 + f3 + 2*f4) * h / 6.;
		double q = 0.25 * (f1 + f2 + f3 + f4) * h;

		double err = std::abs(Q - q), tol = acc + eps * std::abs(Q);
		
		if (err < tol) return std::tuple(Q, err);
		else {
			auto [Q2, err2] = rai(f, 0.5*(a+b), b, acc/std::sqrt(2.), eps, f3, f4);
			auto [Q1, err1] = rai(f, a, 0.5*(a+b), acc/std::sqrt(2.), eps, f1,f2);
			return std::tuple(Q1+Q2, std::sqrt(err1*err1 + err2*err2));
		}
	}

	double erf(double z, double acc, double eps) {
		if (z < 0) return -erf(-z, acc, eps);
		else if (0 <= z && z <= 1) {
			return 2./std::sqrt(std::numbers::pi) * 
				std::get<0>(integrate([](double x){return std::exp(-x*x);}, 0, z, acc, eps));
		}
		else {
			return 1. - 2./std::sqrt(std::numbers::pi) * 
				std::get<0>(integrate([z](double x) {
							double a = z + (1.-x)/x;
							return std::exp(-a*a) / (x*x);
						},
					0, 1, acc, eps));
		}
	}

	std::tuple<double,double> Clenshaw_Curtis_quad(const std::function<double(double)>& f, double a, double b, double acc, double eps) {
		auto [f_,a_,b_] = transform_inf_interval(f, a, b);

		auto transformed_f = [&f_, a_, b_](double t) {
			double x = 0.5 * (a_+b_) + 0.5*(b_-a_)*std::cos(t);
			return f_(x) * std::sin(t) * 0.5 * (b_-a_);
		};

		double h = std::numbers::pi;
		double f2 = transformed_f(2*h / 6.);
		double f3 = transformed_f(4*h / 6.);

		return rai(transformed_f, 0, std::numbers::pi, acc, eps, f2, f3);
	}

	std::tuple<std::function<double(double)>,double,double> transform_inf_interval(const std::function<double(double)>& f, double a, double b) {
		if (std::isinf(a) && std::isinf(b)) {
			return std::tuple([f](double x) {
						double c = 1 - x*x;
						return f(x/c) * (1 + x*x) / (c*c);
				}, -1, 1);
		}
		else if (std::isinf(b)) {
			return std::tuple([f, a](double x) {
						double c = 1 - x;
						return f(a + x/c) / (c*c);
				}, 0, 1);
		}
		else if (std::isinf(a)) {
			return std::tuple([f, b](double x) {
						double c = 1 - x;
						return f(b + x/c) / (c*c);
				}, -1, 0);
		}
		else return std::tuple(f,a,b);
	}
}
