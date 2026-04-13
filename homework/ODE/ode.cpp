#include <functional>
#include <tuple>
#include <vector>
#include <cmath>
#include <algorithm>
#include "ode.hpp"

namespace pp {
	constexpr double a1 = 0.25, a2 = 3./8., a3 = 12./13., a4 = 1., a5 = 0.5;

	constexpr double 	c10=0.25,
			  		 	c20=3./32.,		c21=9./32.,
					 	c30=1932./2197.,c31=-7200./2197., c32=7296./2197,
						c40=439./216,	c41=-8.,		  c42=3680./513.,	c43=-845./4104.,
						c50=-8./27.,	c51=2.,			  c52=-3544./2565.,	c53=1859./4104.,	c54=-11./40.;

	constexpr double p10 = 16./135., p11 = 0., p12 = 6656./12825., p13 = 28561./56430., p14 = -9./50., p15 = 2./55.;
	constexpr double p20 = 25./216., p21 = 0., p22 = 1408./2565.,  p23 = 2197./4104., p24 = -1./5.;

	std::tuple<Vector<double>, Vector<double>> rkstep45(
			const std::function<Vector<double>(double, const Vector<double>&)> f,
			double x,
			const Vector<double>& y,
			double h
	) {
		Vector<double> k0 = f(x, y);
		Vector<double> k1 = f(x + a1*h, y + c10*k0*h);
		Vector<double> k2 = f(x + a2*h, y + c20*k0*h + c21*k1*h);
		Vector<double> k3 = f(x + a3*h, y + c30*k0*h + c31*k1*h + c32*k2*h);
		Vector<double> k4 = f(x + a4*h, y + c40*k0*h + c41*k1*h + c42*k2*h + c43*k3*h);
		Vector<double> k5 = f(x + a5*h, y + c50*k0*h + c51*k1*h + c52*k2*h + c53*k3*h + c54*k4*h);

		Vector<double> y_next = y + h*(p10*k0 + p11*k1 + p12*k2 + p13*k3 + p14*k4 + p15*k5);
		Vector<double> y_err = h*((p10-p20)*k0 + (p11-p21)*k1 + (p12-p22)*k2 + (p13-p23)*k3 + (p14-p24)*k4 + p15*k5);

		return std::tuple(y_next, y_err);
	}

	std::tuple<std::vector<double>, std::vector<Vector<double>>> driver(
			std::function<Vector<double>(double, const Vector<double>&)> f,
			double a,
			double b,
			const VectorBase<double>& y0,
			double h,
			double acc,
			double eps
	) {
		double x = a;
		Vector<double> y(y0);
		std::vector<Vector<double>> y_out;
		std::vector<double> x_out;

		while (x <= b) {
			if (x + h > b) h = b - x;
			
			auto [y_next, y_err] = rkstep45(f, x, y, h);

			double tol = (acc + eps+norm(y_next)) * std::sqrt(h/(b-a));
			double err = norm(y_err);

			if (err <= tol) {
				x += h;
				y = y_next;
				x_out.push_back(x);
				y_out.push_back(y);
			}

			if (err != 0) h *= std::min(std::pow(tol/err, 0.25) * 0.95, 2.);
			else h *= 2.;
		}

		return std::tuple(x_out, y_out);
	}
}
