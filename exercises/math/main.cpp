#include <cmath>
#include <iostream>
#include <numbers>
#include <complex>
#include "sfuns.hpp"

int main () {
	double e = std::numbers::e;
	double pi = std::numbers::pi;
	std::complex<double> I = std::complex<double>(0,1);

	double sqrt2 = std::sqrt(2.0);
	std::cout << "sqrt(2) = " << sqrt2 << "\n";

	double two_root_five = std::pow(2.0, 0.2);
	std::cout << "2^(1/5) = " << two_root_five << "\n"; 

	double e_to_pi = std::pow(e, pi);
	std::cout << "e^pi =" << e_to_pi << "\n";

	double pi_to_e = std::pow(pi, e);
	std::cout << "pi^e =" << pi_to_e << "\n";

	std::cout << "pi^i = " << std::pow(pi,I) << "\n";
	std::cout << "e^i = " << std::pow(e, I) << "\n";
	std::cout << "i^i = " << std::pow(I,I) << "\n";
	std::cout << "log(i) = " << std::log(I) << "\n";

	std::cout << "Gamma func, sfuns (cmath)\n";
	for (size_t i = 1; i <= 10; i++) {
		std::cout << std::format("Γ({}) = {:.6} ({:.6})\n", i, sfuns::fgamma(i), std::tgamma(i));
	}

	std::cout << "\nlngamma, sfuns (cmath)\n";
	for (size_t i = 1; i <= 10; i++) {
		std::cout << std::format("ln(Γ({})) = {:.6} ({:.6})\n", i, sfuns::lngamma(i), std::lgamma(i));
	}

	return 0;
}
