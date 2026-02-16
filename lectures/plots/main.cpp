#include <format>
#include <ios>
#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>

int main (int argc, char **argv) {
	double xmin = 0, xmax = 10, dx = 0.125;
	for (int i = 0; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "--xmin" && ++i < argc) xmin = std::stod(argv[i]);
		else if (arg == "--xmax" && ++i < argc) xmax = std::stod(argv[i]);
		else if (arg == "--dx" && ++i < argc) dx = std::stod(argv[i]);
	}

	std::cerr << std::format("xmin = {}\nxmax = {}\ndx = {}\n", xmin, xmax, dx);
	
	std::cout << std::scientific;

	for (double x = xmin; x <= xmax; x += dx) {
		std::cout << x << " " << std::erf(x) << " " << std::sin(x) << "\n";
	}
	return 0;
}
