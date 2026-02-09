#include <cmath>
#include <format>
#include <iostream>
#include <limits>
#include <iomanip>
#include "approx.hpp"

int main() {
	std::cout << std::fixed << std::setprecision(30);
	// Opg. 1
	
	std::cout << "Opg.1\n";
	std::cout << "                     Computed                          std::numeric_limits               Power of 2\n";
	float float_epsilon = 1.f;
	while ((float) (1.f + float_epsilon) != 1.f) float_epsilon *= 0.5f; float_epsilon *= 2.f;
	std::cout << "float epsilon:       " << float_epsilon << ", "
	 << std::numeric_limits<float>::epsilon() << ", "
	 << std::pow(2, -23) << "\n";

	double double_epsilon = 1.;
	while ((double) (1. + double_epsilon) != 1.) double_epsilon *= 0.5; double_epsilon *= 2.;
	std::cout << "double epsilon:      " << double_epsilon << ", "
	 << std::numeric_limits<double>::epsilon() << ", "
	 << std::pow(2, -52) << "\n";

	long double long_double_epsilon = 1.;
	while ((long double) (1.L + long_double_epsilon) != 1.L) long_double_epsilon *= 0.5L; long_double_epsilon *= 2.L;
	std::cout << "long double epsilon: " << long_double_epsilon << ", "
	 << std::numeric_limits<long double>::epsilon() << "\n";

	std::cout << "The computed ones are the same as the ones in std::numeric_limits\n";
	// std::cout << std::format("{}\n", double_epsilon == std::numeric_limits<double>::epsilon());

	// Opg. 2
	std::cout << std::fixed << std::setprecision(17);
	std::cout << "\nOpg. 2\n";
	// double epsilon = std::numeric_limits<double>::epsilon();
	double epsilon = std::pow(2, -52);
	double tiny = epsilon / 2.;
	double a = 1. + tiny + tiny;
	std::cout << "a = 1. + tiny + tiny = " << a << "\n";

	double b = tiny + tiny + 1.;
	
	std::cout << "b = tiny + tiny + 1. = " << b << "\n";

	std::cout << std::format("a == b: {}", a == b) << "\n";
	std::cout << std::format("a < b: {}", a < b) << "\n";
	std::cout << std::format("a > b: {}", a > b) << "\n";

	std::cout << "This is due to precision being better at numbers close to 0,\n\
so (tiny + tiny) + 1. = epsilon + 1. != 1. (by definition.)\n\
However (1. + tiny) + tiny = 1. + tiny = 1.\n";

	// Opg. 3
	std::cout << "\nOpg. 3\n";
	double d1 = 0.1+0.1+0.1+0.1+0.1+0.1+0.1+0.1;
	double d2 = 8*0.1;
	std::cout << "d1==d2?: " << (d1==d2 ? "true":"false") << "\n"; 

	std::cout << "approx(d1,d2): " << std::format("{}\n", approx::approx(d1, d2));

	return 0;
}
