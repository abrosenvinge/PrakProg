#include "splines.hpp"
#include <cmath>
#include <functional>
#include <numbers>
#include <string>
#include <vector>
#include <iostream>

constexpr double PI = std::numbers::pi;

void test_qs() {
	std::vector<double> x{1,2,3,4,5};
	std::vector<double> y1{1,1,1,1,1};

	pp::QuadraticSpline<double> qs1(x,y1);
	std::cout << "Test for constant function:\n";
	std::cout << "	xi = i\n	yi = 1\n";
	std::cout << "	Expected: bi = 0, ci = 0\n";
	std::cout << "	Calculated:\n";
	std::cout << std::format("		b = {}\n", qs1.b);
	std::cout << std::format("		c = {}\n\n", qs1.c);

	std::vector<double> y2{1,2,3,4,5};
	pp::QuadraticSpline<double> qs2(x,y2);
	std::cout << "Test for linear function:\n";
	std::cout << "	xi = i\n	yi = xi\n";
	std::cout << "	Expected: bi = 1, ci = 0\n";
	std::cout << "	Calculated:\n";
	std::cout << std::format("		b = {}\n", qs2.b);
	std::cout << std::format("		c = {}\n\n", qs2.c);

	std::vector<double> y3{1,4,9,16,25};
	pp::QuadraticSpline<double> qs3(x,y3);
	std::cout << "Test for quadratic function:\n";
	std::cout << "	xi = i\n	yi = xi^2\n";
	std::cout << "	Expected: bi = 2i, ci = 1\n";
	std::cout << "	Calculated:\n";
	std::cout << std::format("		b = {}\n", qs3.b);
	std::cout << std::format("		c = {}\n", qs3.c);
}

int main(int argc, char** argv) {
	size_t n_points = 10;
	size_t n_interp_points = 100;
	double start = 0;
	double end = 2*PI;
	std::string function_string = "sin";
	bool test = false;

	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "--n_points" && ++i < argc) n_points = std::stoi(argv[i]);
		else if (arg == "--n_interp_points" && ++i < argc) n_interp_points = std::stoi(argv[i]);
		else if (arg == "--start" && ++i < argc) start = std::stod(argv[i]);
		else if (arg == "--end" && ++i < argc) end = std::stod(argv[i]);
		else if (arg == "-f" && ++i < argc) function_string = argv[i];
		else if (arg == "--test") test = true;
	}

	std::function<double(double)> f_deriv;
	std::function<double(double)> f;
	std::function<double(double)> F;

	if (function_string == "exp") {
		f = [](double x) {return std::exp(x); };
		f_deriv = f;
		F = [&start](double x) {return std::exp(x) - std::exp(start); };
	} else if (function_string == "quadratic") {
		f_deriv = [](double x) {return 2*x;};
		f = [](double x) {return x*x; };
		F = [&start](double x) {return (std::pow(x,3) - std::pow(start,3)) / 3; };
	} else {
		f_deriv = [](double x){return -std::sin(x); };
		f = [](double x) {return std::cos(x); };
		F = [&start](double x) {return (std::sin(x) - std::sin(start)); };
	}

	if (test) {
		test_qs();
		return 0;
	}

	std::vector<double> x(n_points), y(n_points);
	double xstep = (end - start) / (n_points-1);
	

	for (size_t i = 0; i < n_points; ++i) {
		double xi = start + i * xstep;
		x[i] = xi;
		y[i] = f(xi);
		std::cout << xi << " " << y[i] << " " << F(xi) << " " << f_deriv(xi) << "\n";
	}
	std::cout << "\n\n";

	// std::vector<double> z(n_interp_points), 
	// 					y_lin(n_interp_points), 
	// 					Y_lin(n_interp_points),
	// 					y_quad(n_interp_points),
	// 					Y_quad(n_interp_points);

	double zstep = (end - start) / (n_interp_points-1);

	pp::LinearSpline<double> lin_spline(x,y);
	pp::QuadraticSpline<double> quad_spline(x,y);
	auto quad_spline_func = pp::quadratic_spline(x,y);

	for (size_t i = 0; i < n_interp_points; ++i) {
		double zi = start + i * zstep;
		std::cout << zi << " " 
				  << lin_spline.eval(zi) << " " 
				  << lin_spline.integral(zi) << " " 
				  << lin_spline.derivative(zi) << " "
				  << quad_spline.eval(zi) << " "
				  << quad_spline.integral(zi) << " " 
				  << quad_spline.derivative(zi) << " "
				  << quad_spline_func(zi)
				  << "\n";
	}
}
