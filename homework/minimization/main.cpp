#include <cmath>
#include <format>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include "minimization.hpp"

void test(std::string name, 
		std::function<double(pp::Vector<double>)>& f,
		std::vector<pp::Vector<double>> x0,
		std::vector<pp::Vector<double>> correct_x)
{

	std::cout << std::format("Test: {}:\n", name);
	std::cout << std::format("	Correct {}:\n", correct_x.size() == 1 ? "minimum" : "minima");
	for (size_t j = 0; j < correct_x[0].size; ++j) {
		std::cout << "		";
		for (size_t i = 0; i < correct_x.size(); ++i) {
			std::cout << std::format("{:<10} ", correct_x[i][j]);
		}
		std::cout << "\n";
	}
	pp::Vector<double> g(x0[0].size);
	for (size_t i = 0; i < x0.size(); ++i) {
		int n_evals = 0, n_evals_fd = 0, n_evals_unopt = 0;
		std::cout << "\n";
		pp::Vector<double> x0i = x0[i];
		pp::MinimizationResult resi_fd = pp::min_newton_fd([&n_evals_fd,f](auto x){n_evals_fd++; return f(x);}, x0i, 0.0001, 0.0001, 10000);
		pp::MinimizationResult resi = pp::min_newton([&n_evals,f](auto x){n_evals++; return f(x);}, x0i, 0.0001, 0.0001, 10000);
		pp::MinimizationResult resi_unopt = pp::min_newton_central_unopt([&n_evals_unopt,f](auto x){n_evals_unopt++; return f(x);}, x0i, 0.0001, 0.0001, 10000);
		std::cout << std::format("		{:<25} {:<25} {:<25} {:<25}\n", "Method:", "Forward diff", "Central diff", "Central diff (unoptimized)");
		std::cout << std::format("		{:<25} {:<25} {:<25} {:<25}\n", "Norm(g):", pp::norm(resi_fd.gfx), pp::norm(resi.gfx), pp::norm(resi_unopt.gfx));
		std::cout << std::format("		{:<25} {:<25} {:<25} {:<25}\n", "#evaluations:", n_evals_fd, n_evals, n_evals_unopt);
		std::cout << std::format("		{:<25} {:<25} {:<25} {:<25}\n", "#iterations:", resi_fd.n_iters, resi.n_iters, resi_unopt.n_iters);
		std::cout << std::format("		{:<25} {:<25}\n", "x0", "Result");
		
		for (size_t j = 0; j < x0i.size; ++j) {
			std::cout << std::format("		{:<25} {:<25} {:<25} {:<25}\n", x0i[j], resi_fd.x[j], resi.x[j], resi_unopt.x[j]);
		}
	}
	std::cout << "\n";
}

void tests() {
	std::function<double(pp::Vector<double>)> f1 = [](pp::Vector<double> x) {
		return x[0] * x[0] + x[1] * x[1];
	};

	// pp::Vector<double> x {1,1};
	// pp::Vector<double> g(2);
	// pp::gradient(f1, x, g);
	// std::cout << g << "\n\n";
	//
	// pp::Matrix<double> H(2,2);
	// pp::hessian(f1, x, H);
	// std::cout << H << "\n\n";

	test("f(x,y) = x^2 + y^2", f1, {{1,2}, {3,4}}, {{0,0}});


	std::function<double(pp::Vector<double>)> rosenbrock = 
		[](pp::Vector<double> z) {
			double x = z[0], y = z[1];
			double out = std::pow(1-x*x,2) + 100.*std::pow(y-x*x,2);
			return out;
	};
	test("Rosenbrock", rosenbrock, {{1.5,0.5}, {3,4}}, {{1,1}});

	std::function<double(pp::Vector<double>)> himmelblau = 
		[](pp::Vector<double> z) {
			double x = z[0], y = z[1];
			double out = std::pow(x*x+y-11.,2) + std::pow(x+y*y-7.,2);
			return out;
	};
	test("Himmelblau", himmelblau, 
			{{2.5,4}, {-2,2},{-5,-5},{5,-3}}, 
			{{3.,2.}, {-2.805118, 3.131312}, 
				{-3.779310, -3.283186},
				{3.584428, -1.848126}});
}

void higgs_fit(double acc, size_t n_points, pp::Vector<double> p0) {
	std::vector<double> energy, signal, error;
	double x,y,sy;
	while (std::cin >> x >> y >> sy) {
		energy.push_back(x);
		signal.push_back(y);
		error.push_back(sy);
	}
	
	auto f = [](double E, pp::Vector<double> p) {
		double m = p[0], g = p[1], A = p[2];
		return A/(std::pow(E-m,2) + g*g/4.);
	};

	pp::MinimizationResult res = pp::leastsq_fit(f, energy, signal, error, p0, acc);

	pp::Vector<double> p = res.x;
	// pp::Vector<double> p = p0;

	double E = energy[0], Emax = energy.back(), step_size = (Emax-E)/n_points;

	for(size_t i = 0; i < n_points; ++i) {
		std::cout << std::format("{} {}\n", E, f(E,p));
		E += step_size;
	}

	std::cerr << "Higgs fit:\n";
	std::cerr << "	Optimal parameters\n";
	std::cerr << std::format("		m = {}\n", res.x[0]);
	std::cerr << std::format("		Γ = {}\n", res.x[1]);
	std::cerr << std::format("		A = {}\n\n", res.x[2]);

	std::cerr << std::format("	χ² = {}\n", res.fx);
	std::cerr << std::format("	dof ={}\n", energy.size() - p.size);
}

int main(int argc, char** argv) {
	double acc = 0.0000001;
	size_t n_points = 10000;
	pp::Vector<double> p0 {1,2,3};

	for (int i = 0; i < argc; ++i) {
		std::string arg = argv[i];
		
		if (arg == "--acc" && ++i < argc) acc = std::stod(argv[i]);
		else if (arg == "--plot_steps" && ++i < argc) n_points = std::stoi(argv[i]);
		else if (arg == "--p0" && i + 3 < argc) {
			p0[0] = std::stod(argv[i+1]);
			p0[1] = std::stod(argv[i+2]);
			p0[2] = std::stod(argv[i+3]);
			i += 3;
		}
		else if (arg == "--test") {
			tests();
		}
		else if (arg == "--higgs") {
			higgs_fit(acc, n_points, p0);
		}
	}
}
