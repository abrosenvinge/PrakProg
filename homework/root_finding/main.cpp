#include "root_finding.hpp"
#include <cmath>
#include <format>
#include <functional>
#include <iostream>
#include <limits>
#include <string>
#include "../ODE/ode.hpp"
// #include "../lib/matrix.hpp"

void test(std::string name, 
		std::function<pp::Vector<double>(pp::Vector<double>)>& f,
		std::vector<pp::Vector<double>> x0,
		std::vector<pp::Vector<double>> correct_x)
{
	auto res = pp::newton(f, x0, 0.01, 0.001, 10000);

	std::cout << std::format("Test: {}:\n", name);
	std::cout << "	Correct root(s):\n";
	for (size_t j = 0; j < correct_x[0].size; ++j) {
		std::cout << "		";
		for (size_t i = 0; i < correct_x.size(); ++i) {
			std::cout << std::format("{:<15} ", correct_x[i][j]);
		}
		std::cout << "\n";
	}
	for (size_t i = 0; i < res.size(); ++i) {
		std::cout << "\n";
		pp::Vector<double> x0i = x0[i];
		pp::Vector<double> resi = res[i];
		std::cout << std::format("	Method: {}\n", "Newton");
		std::cout << std::format("	Norm: {}\n", norm(f(resi)));
		std::cout << std::format("		{:<15} {:<15}\n", "x0", "Result");
		
		for (size_t j = 0; j < resi.size; ++j) {
			std::cout << std::format("		{:<15} {:<15}\n", x0i[j], resi[j]);
		}
	}
}

void tests() {
	std::function<pp::Vector<double>(pp::Vector<double>)> id = [](pp::Vector<double> x) {
		return x;
	};
	test("id(x) = x", id, {{1,2}, {500, -719}}, {{0,0}});

	std::function<pp::Vector<double>(pp::Vector<double>)> f1 = [](pp::Vector<double> x) {
		x[0] *= x[0]; x[1] *= x[1];
		return x;
	};
	test("f(x,y) = (x^2, y^2)", f1, {{1,2}, {3,4}}, {{0,0}});


	std::function<pp::Vector<double>(pp::Vector<double>)> grad_rosenbrock = 
		[](pp::Vector<double> z) {
			pp::Vector<double> out(z.size);
			
			double x = z[0], y = z[1];

			out[0] = 2. - 2.*x - 400. * x * (y - x*x);
			out[1] = 200. * (y - x*x);

			return out;
	};
	test("Gradient of Rosenbrock", grad_rosenbrock, {{1.5,0.5}, {3,4}}, {{1,1}});

	std::function<pp::Vector<double>(pp::Vector<double>)> grad_himmelblau = 
		[](pp::Vector<double> z) {
			pp::Vector<double> out(z.size);
			
			double x = z[0], y = z[1];

			out[0] = 4.*x*(x*x + y - 11.) + 2.*(x+y*y-7.);
			out[1] = 2.*(x*x+y-11.) + 4.*y*(x+y*y-7);

			return out;
	};
	test("Gradient of Himmelblau", grad_himmelblau, 
			{{1.5,0.5}, {2.5,4}, {-1,1},{-5,-5},{5,-3}}, 
			{{-0.270845, -0.923039}, {3.,2.}, {-2.805118, 3.131312}, 
				{-3.779310, -3.283186},
				{3.584428, -1.848126}});
}

void shooting_method_pprint(double rmin, double rmax, double E0) {
	double acc = 0.0000001, eps = 0.0000001;
	double E = pp::shooting_method_hydrogen(rmin,rmax,E0,acc,eps);
	pp::Vector<double> y0 {rmin-rmin*rmin, 1.-2.*rmin};
	// std::cerr << E << " " << M({E}) << "\n";

	double M = pp::endpoint_driver(pp::construct_ode(E), rmin, rmax, y0, 0.125, acc, eps)[0];
	
	std::cout << "Shooting method:\n";
	std::cout << std::format("	rmin = {}, rmax = {}, E_guess = {}\n", rmin, rmax, E0);
	std::cout << std::format("	Calculated E0: {}\n", E);
	std::cout << std::format("	M(E0) = {}\n", M);
}

void shooting_method_wavefunc(double rmin, double rmax, double E0) {
	double acc = 0.0000001, eps = 0.0000001;
	double E = pp::shooting_method_hydrogen(rmin,rmax,E0,acc,eps);
	pp::Vector<double> y0 {rmin-rmin*rmin, 1.-2.*rmin};

	auto [x,y] = pp::driver(pp::construct_ode(E), rmin, rmax, y0, 0.125, acc, eps);

	for (size_t i = 0; i < x.size(); ++i) {
		std::cout << x[i] << " " << y[i][0] << " " << x[i] * std::exp(-x[i]) << "\n";
	}
}

int main(int argc, char** argv) {
	for (int i = 0; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "--test") {
			tests();
			std::cout << "\n";
			shooting_method_pprint(0.0000001, 8., -0.6);
		}
		else if (arg == "--shoot" && i + 4 < argc) {
			double rmin = std::stod(argv[i+1]), rmax = std::stod(argv[i+2]),
				   acc = std::stod(argv[i+3]), eps = std::stod(argv[i+4]);
			i += 4;
		}
		else if (arg == "--wavefunc" && i + 2 < argc) {
		}
	}
}
