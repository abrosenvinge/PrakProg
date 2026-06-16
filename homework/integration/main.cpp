#include "integration.hpp"
#include <cmath>
#include <format>
#include <functional>
#include <iostream>
#include <string>

void test(std::string fstr, double a, double b) {
	int n_evals = 0;
	std::function<double(double)> f;
	if (fstr == "1/sqrt(x)") 
		f = [&n_evals](double x) {n_evals++; return 1/std::sqrt(x);};
	else if (fstr == "ln(x)/sqrt(x)")
		f = [&n_evals](double x) {n_evals++; return std::log(x) / std::sqrt(x);};
	else if (fstr == "sqrt(x)") 
		f = [&n_evals](double x) {n_evals++; return std::sqrt(x);};
	else if (fstr == "sqrt(1-x*x)") 
		f = [&n_evals](double x) {n_evals++; return std::sqrt(1 - x*x);};
	else if (fstr == "exp(-x)")
		f = [&n_evals](double x) {n_evals++; return std::exp(-x);};
	else if (fstr == "exp(-x*x)")
		f = [&n_evals](double x) {n_evals++; return std::exp(-x*x);};

	auto [res1,err1] = pp::integrate(f, a, b);
	std::cout << std::format("Ordinary {} {} {}\n", res1, n_evals, err1);

	n_evals = 0;
	auto [res2, err2] = pp::Clenshaw_Curtis_quad(f, a, b);
	std::cout << std::format("CC {} {} {}\n", res2, n_evals, err2);
}

void erf_plot_data(double start, double stop, int n_points) {
	double z = start;
	double step = (stop - start) / n_points;

	for (int i = 0; i < n_points; ++i) {
		std::cout << std::format("{} {} {}\n", z, pp::erf(z), std::erf(z));
		z += step;
	}
}

void erf_error_data(double start_acc, double stop_acc, int n_points) {
	double acc = start_acc;
	double erf1 = std::erf(1.);
	double factor = std::pow(start_acc/stop_acc, 1./(n_points-1));

	for (int i = 0; i < n_points; ++i) {
		std::cout << std::format("{} {}\n", acc, std::abs(pp::erf(1., acc, 0.) - erf1));
		acc /= factor;
	}
}

int main(int argc, char** argv) {
	for (int i = 0; i < argc; ++i) {
		std::string arg(argv[i]);
		if (arg == "--erf" && i + 3 < argc) {
			double start = std::stod(argv[i+1]), stop = std::stod(argv[i+2]);
			int n_points = std::stoi(argv[i+3]);
			erf_plot_data(start, stop, n_points);
			i += 3;
		}
		else if (arg == "--erf_error" && i + 3 < argc) {
			double start = std::stod(argv[i+1]), stop = std::stod(argv[i+2]);
			int n_points = std::stoi(argv[i+3]);
			erf_error_data(start, stop, n_points);
			i += 3;
		}
		else if (arg == "--test" && i + 3 < argc) {
			std::string fstr = argv[i+1];
			double a = std::stod(argv[i+2]), b = std::stod(argv[i+3]);
			test(fstr, a, b);
			i += 3;
		}
	}
	
	return 0;
}
