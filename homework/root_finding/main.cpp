#include "root_finding.hpp"
#include <cmath>
#include <format>
#include <functional>
#include <iostream>
#include <string>
#include "../ODE/ode.hpp"
// #include "../lib/matrix.hpp"

void test(std::string name, 
		std::function<pp::Vector<double>(pp::Vector<double>)>& f,
		std::vector<pp::Vector<double>> x0,
		std::vector<pp::Vector<double>> correct_x)
{

	std::cout << std::format("Test: {}:\n", name);
	std::cout << "	Correct root(s):\n";
	for (size_t j = 0; j < correct_x[0].size; ++j) {
		std::cout << "		";
		for (size_t i = 0; i < correct_x.size(); ++i) {
			std::cout << std::format("{:<10} ", correct_x[i][j]);
		}
		std::cout << "\n";
	}
	for (size_t i = 0; i < x0.size(); ++i) {
		int n_evals = 0, n_evals_qls = 0;
		std::cout << "\n";
		pp::Vector<double> x0i = x0[i];
		auto resi = pp::newton([&n_evals,f](auto x){n_evals++; return f(x);}, x0i, 0.0000001, 0.0001, 100000);
		auto resi_qls = pp::newton_qls([&n_evals_qls,f](auto x){n_evals_qls++; return f(x);}, x0i, 0.0000001, 0.0001, 100000);
		std::cout << std::format("		{:<25} {:<25} {:<25}\n", "Method:", "Newton", "QLS");
		std::cout << std::format("		{:<25} {:<25} {:<25}\n", "Norm:", norm(f(resi)), norm(f(resi_qls)));
		std::cout << std::format("		{:<25} {:<25} {:<25}\n", "#evaluations:", n_evals, n_evals_qls);
		std::cout << std::format("		{:<25} {:<25}\n", "x0", "Results");
		
		for (size_t j = 0; j < resi.size; ++j) {
			std::cout << std::format("		{:<25} {:<25} {:<25}\n", x0i[j], resi[j], resi_qls[j]);
		}
	}
	std::cout << "\n";
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

void shooting_method_pprint(double rmin, double rmax, double E0, double acc, double eps) {
	double E = pp::shooting_method_hydrogen(rmin,rmax,E0,acc,eps);
	pp::Vector<double> y0 {rmin-rmin*rmin, 1.-2.*rmin};
	// std::cerr << E << " " << M({E}) << "\n";

	double M = pp::endpoint_driver(pp::construct_ode(E), rmin, rmax, y0, 0.125, acc, eps)[0];
	
	std::cout << "Shooting method:\n";
	std::cout << std::format("	rmin = {}, rmax = {}, E_guess = {}\n", rmin, rmax, E0);
	std::cout << std::format("	Calculated E0: {}\n", E);
	std::cout << std::format("	M(E0) = {}\n", M);
}

void shooting_method_wavefunc(double rmin, double rmax, double E0, double acc, double eps) {
	double E = pp::shooting_method_hydrogen(rmin,rmax,E0,acc,eps);
	pp::Vector<double> y0 {rmin-rmin*rmin, 1.-2.*rmin};

	auto [x,y] = pp::driver(pp::construct_ode(E), rmin, rmax, y0, 0.125, acc, eps);

	for (size_t i = 0; i < x.size(); ++i) {
		std::cout << x[i] << " " << y[i][0] << "\n";
	}
}

void actual_wavefunc(double rmin, double rmax, int steps) {
	double h = (rmax - rmin) / steps;
	double r = rmin;
	for (int i = 0; i < steps; ++i) {
		std::cout << std::format("{} {}\n", r, r * std::exp(-r));
		r += h;
	}
}

int main(int argc, char** argv) {
	double rmin = 0.0000001, rmax = 8., acc = 0.0000001, eps = 0.0000001, E0 = -0.6;
	int steps = 1000;
	for (int i = 0; i < argc; ++i) {
		std::string arg = argv[i];
		
		if (arg == "--rmax" && ++i < argc) rmax = std::stod(argv[i]);
		else if (arg == "--rmin" && ++i < argc) rmin = std::stod(argv[i]);
		else if (arg == "--acc" && ++i < argc) acc = std::stod(argv[i]);
		else if (arg == "--eps" && ++i < argc) eps = std::stod(argv[i]);
		else if (arg == "--E0" && ++i < argc) E0 = std::stod(argv[i]);
		else if (arg == "--steps" && ++i < argc) steps = std::stoi(argv[i]);
		else if (arg == "--test") {
			tests();
			std::cout << "\n";
			shooting_method_pprint(rmin, rmax, E0, acc, eps);
		}
		else if (arg == "--shoot") {
			std::cout << std::format("{}\n", pp::shooting_method_hydrogen(rmin, rmax, E0, acc, eps));
		}
		else if (arg == "--wavefunc") {
			shooting_method_wavefunc(rmin, rmax, E0, acc, eps);
		}
		else if (arg == "--actual_wavefunc") {
			actual_wavefunc(rmin, rmax, steps);
		}
	}
}
