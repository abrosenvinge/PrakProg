#include "monte_carlo.hpp"
#include <cmath>
#include <iostream>
#include <numbers>
#include <string>
#include <vector>
#include <functional>

void pprint_test(std::string name,
				std::function<double(pp::Vector<double>)> f, 
				pp::Vector<double> a, 
				pp::Vector<double> b, 
				double solution) {
	pp::LCG lcg_rand(74804);
	pp::std_uniform_dist uniform_rand;

	size_t N = 100000;

	auto [res_lcg, err_lcg] = pp::plainmc(f, a, b, N, lcg_rand);
	auto [res_uniform, err_uniform] = pp::plainmc(f, a, b, N, uniform_rand);
	auto [res_quasi, err_quasi] = pp::quasirandmc(f, a, b, N);
	auto [res_strati, err_strati] = pp::stratified(f, a, b, N, 100);

	std::cout << name << "\n";
	std::cout << std::format("	{:<25}{:<25}{:<25}{:<25}\n", "Random method", "Calculated", "Est. error", "Actual error");
	std::cout << std::format("	{:<25}{:<25}\n", "Solution", solution);
	std::cout << std::format("	{:<25}{:<25}{:<25}{:<25}\n", "lcg", res_lcg, err_lcg, std::abs(solution - res_lcg));
	std::cout << std::format("	{:<25}{:<25}{:<25}{:<25}\n", "uniform", res_uniform, err_uniform, std::abs(solution - res_uniform));
	std::cout << std::format("	{:<25}{:<25}{:<25}{:<25}\n", "quasi", res_quasi, err_quasi, std::abs(solution - res_quasi));
	std::cout << std::format("	{:<25}{:<25}{:<25}{:<25}\n", "stratified", res_strati, err_strati, std::abs(solution - res_strati));
}

void scaling_test(size_t start, size_t stop, size_t step, 
					std::function<double(pp::Vector<double>)> f, 
					pp::Vector<double> a, 
					pp::Vector<double> b, 
					double solution
				) 
{
	pp::LCG lcg_rand(74804);
	pp::std_uniform_dist uniform_rand;

	size_t N = start;
	while (N <= stop) {
		auto [res_lcg, err_lcg] = pp::plainmc(f, a, b, N, lcg_rand);
		auto [res_uniform, err_uniform] = pp::plainmc(f, a, b, N, uniform_rand);
		auto [res_quasi, err_quasi] = pp::quasirandmc(f, a, b, N);
		
		std::cout << std::format("{} {} {} {} {} {} {} {} {} {} {}\n", N, solution, 
									res_lcg, err_lcg, std::abs(solution - res_lcg), 
									res_uniform, err_uniform, std::abs(solution - res_uniform),
									res_quasi, err_quasi, std::abs(solution - res_quasi));
		N += step;
	}
}

int main(int argc, char** argv) {
	auto f_unit_circle = [](const pp::Vector<double>& x) {
		double x0 = x[0], x1 = x[1];
		return (double) ((x0*x0 + x1*x1) <= 1.);
	};
	double a_unit_circle = std::numbers::pi;

	auto f_disk_potential = [](const pp::Vector<double>& x) {
		double x0 = x[0], x1 = x[1];
			return 1./std::sqrt(x0*x0 + x1*x1 + 1) * ((x0*x0 + x1*x1) <= 1.);
	}; // ~electric potential above disk. should be 2pi(sqrt(2) - 1)
	double a_disk_potential = 2.*std::numbers::pi*(std::sqrt(2) - 1);

	double a=1., b=2., c=3.;
	auto f_ellipsoid = [a,b,c](const pp::Vector<double>& x) {
		double x0 = x[0], x1 = x[1], x2 = x[2];
		return (double) (((x0*x0)/(a*a) + (x1*x1)/(b*b) + (x2*x2)/(c*c)) <= 1.);
	};
	double a_ellipsoid = 4.*std::numbers::pi*a*b*c/3.;

	auto f_difficult = [](const pp::Vector<double>& x) {
		return 1./(std::pow(std::numbers::pi,3) * (1 - std::cos(x[0])*std::cos(x[1])*std::cos(x[2])));
	};
	double a_difficult = 1.3932039296856768591842462603255;

	// function that is at least a little assymetric which may benefit the stratified method
	auto f_gauss = [](const pp::Vector<double>& x) {
		return std::exp(-std::pow(x[0]-1,2) - std::pow(x[1]+1,2));
	};
	double a_gauss = 0.25*std::numbers::pi*std::pow((std::erf(1.)+std::erf(3.)),2);
	
	for (int i = 0; i < argc; ++i) {
		std::string arg(argv[i]);
		if (arg == "--scaling" && i + 3 < argc) {
			size_t start = std::stoi(argv[i+1]), stop = std::stoi(argv[i+2]),
			step = std::stoi(argv[i+3]);
			i += 3;
			scaling_test(start, stop, step, f_unit_circle, {-1, -1}, {1,1}, a_unit_circle);
			std::cout << "\n\n";
			scaling_test(start, stop, step, f_disk_potential, {-1, -1}, {1,1}, a_disk_potential);
		}
		else if (arg == "--pprint") {
			pprint_test("Unit circle", f_unit_circle, {-1,-1}, {1, 1}, a_unit_circle);
			std::cout << "\n\n";
			pprint_test("Disk potential", f_disk_potential, {-1,-1}, {1, 1}, a_disk_potential);
			std::cout << "\n\n";
			pprint_test("Ellipsoid", f_ellipsoid, {-a,-b,-c}, {a, b, c}, a_ellipsoid);
			std::cout << "\n\n";
			pprint_test("Difficult integral", f_difficult, {0,0,0}, {std::numbers::pi,std::numbers::pi,std::numbers::pi}, a_difficult);
			std::cout << "\n\n";
			pprint_test("Gaussian", f_gauss, {-2,-2}, {2,2}, a_gauss);
		}
	}


	// for (int i = 0; i < 1000; ++i) std::cout << lcg_rand() << "\n";
}
