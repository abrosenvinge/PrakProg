#include "ode.hpp"
#include "../lib/matrix.hpp"
#include <iostream>
#include <limits>
#include <string>

int main(int argc, char** argv) {
	std::string mode = "shm";
	double start = 0.;
	double end = 2.;
	double acc = 0.01;
	double eps = 0.01;
	double initial_step = 0.125;
	double epsilon = 0.;
	double max_step = std::numeric_limits<double>::infinity();
	bool read_y0 = false;
	pp::Vector<double> y0;

	for (int i = 0; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "--mode" && ++i < argc) mode = argv[i];
		else if (arg == "--start" && ++i < argc) start = std::stod(argv[i]);
		else if (arg == "--end" && ++i < argc) end = std::stod(argv[i]);
		else if (arg == "--acc" && ++i < argc) acc = std::stod(argv[i]);
		else if (arg == "--eps" && ++i < argc) eps = std::stod(argv[i]);
		else if (arg == "--initial_step" && ++i < argc) initial_step = std::stod(argv[i]);
		else if (arg == "--epsilon" && ++i < argc) epsilon = std::stod(argv[i]);
		else if (arg == "--max_step" && ++i < argc) max_step = std::stod(argv[i]);
		else if (arg == "--y0" && ++i < argc) {
			read_y0 = true;
			y0.push_back(std::stod(argv[i]));
		}
	}

	std::function<pp::Vector<double>(double, const pp::Vector<double>&)> f;

	// simple harmonic oscillator
	if (mode == "shm") {
		// u'' = -u as y0' = y1, y1' = -y0
		f = [](double, const pp::Vector<double>& y) {
			pp::Vector<double> out = y;
			out[0] = y[1];
			out[1] = -y[0];
			return out;
		};

		if (!read_y0) {
			y0 = pp::Vector<double>(2);
			y0[0] = 1.;
			y0[1] = 0.;
		}
	}
	else if (mode == "lotka_volterra") {
		double a = 1.5, b = 1., c = 3., d = 1.;
		f = [&a,&b,&c,&d](double, const pp::Vector<double>& y) {
			pp::Vector<double> out = y;
			out[0] = a*y[0] - b*y[0]*y[1];
			out[1] = -c*y[1] + d*y[0]*y[1];
			return out;
		};

		if (!read_y0) {
			y0 = pp::Vector<double>(2);
			y0[0] = 10.;
			y0[1] = 5.;
		}
	}
	else if (mode == "rel_orbit") {
		f = [&epsilon](double, const pp::Vector<double>& y) {
			pp::Vector<double> out = y;
			double y0 = y[0];
			out[0] = y[1];
			out[1] = -y0 + 1 + epsilon * y0*y0;
			return out;
		};

		if (!read_y0) {
			y0 = pp::Vector<double>(2);
			y0[0] = 10.;
			y0[1] = 5.;
		}
	}
	auto [x, y] = pp::driver(f, start, end, y0, initial_step, acc, eps, max_step);

	for (size_t i = 0; i < x.size(); ++i) {
		std::cout << x[i];
		pp::Vector<double> yi = y[i];
		for (size_t j = 0; j < yi.size; ++j) std::cout << " " << yi[j];
		std::cout << "\n";
	}
	std::cout << "\n" << std::endl;
}
