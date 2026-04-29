#include "integration.hpp"
#include <cmath>
#include <iostream>


int main() {
	auto f1 = [](double x) {return std::sqrt(x);};
	auto f2 = [](double x) {return 1/std::sqrt(x);};
	auto f3 = [](double x) {return std::sqrt(1 - x*x);};
	auto f4 = [](double x) {return std::log(x) / std::sqrt(x);};

	std::cout << "int sqrt(x) from 0 to 1 = " << pp::integrate(f1, 0, 1) << "\n";
	std::cout << "int 1/sqrt(x) from 0 to 1 = " << pp::integrate(f2, 0, 1) << "\n";
	std::cout << "int sqrt(1-x*x) from 0 to 1 = " << pp::integrate(f3, 0, 1) << "\n";
	std::cout << "int ln(x)/sqrt(x) from 0 to 1 = " << pp::integrate(f4, 0, 1) << "\n";
	
	return 0;
}
