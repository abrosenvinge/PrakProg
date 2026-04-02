#include "least_squares.hpp"
#include <cmath>
#include <iostream>
#include <functional>
#include "../lib/matrix.hpp"
#include <vector>

int main() {
	pp::Vector<double> t(0), y(0), dy(0);
	{
		double t_in, y_in, dy_in;

		while (std::cin >> t_in >> y_in >> dy_in) {
			t.push_back(t_in);
			y.push_back(y_in);
			dy.push_back(dy_in);
		}
	}

	pp::Vector<double> lny(y.size), dlny(dy.size);

	for (size_t i = 0; i < lny.size; ++i) {
		lny[i] = std::log(y[i]);
		dlny[i] = dy[i] / y[i];
	}

	auto fs = std::vector<std::function<double(double)>>{
		[](double) {return 1.;},
		[](double x) {return -x;}
	};

	auto [c, covariance_matrix] = pp::lsfit(fs, t, lny, dlny);

	double a = std::exp(c[0]); // since the fitted parameter is ln(a)
	double da = a * std::sqrt(covariance_matrix[0,0]);

	double lambda = c[1];
	double dlambda = std::sqrt(covariance_matrix[1,1]);

	double Thalf = std::log(2) / lambda;
	double dThalf = std::log(2) / (lambda * lambda) * dlambda;

	std::cerr << "a = " << a << " ± " << da << "\n";
	std::cerr << "λ = " << lambda << " ± " << dlambda << "\n";
	std::cerr << "T½ = " << Thalf << " ± " << dThalf << "\n";
	std::cerr << "actual_half_life = " << 3.6313 << "\n"; // from https://www.nndc.bnl.gov/nudat3/
	std::cerr << "Not within the estimated uncertainty" << "\n";

	std::cerr << "Σ =\n" << covariance_matrix;

	std::cout << "a = " << a << "\n";
	std::cout << "da = " << da << "\n";
	std::cout << "lambda = " << lambda << "\n";
	std::cout << "dlambda = " << dlambda << "\n\n";
}
