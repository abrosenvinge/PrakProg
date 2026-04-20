#include "ode.hpp"
#include "../lib/matrix.hpp"
#include <iostream>
#include <limits>
#include <numbers>
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
			y0.push_back(std::stod(argv[i]));
		}
	}
	bool read_y0 = y0.size > 0;

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
	else if (mode == "three_body_problem") {
		f = [](double, const pp::Vector<double>& z) {
			pp::Vector<double> out(z.size);

			for (int i = 0; i < 6; i += 2) {
				for (int k = 2; k < 6; k += 2) {
					int j = ((i + k) % 6);
					double dx = z[j+6] - z[i+6];
					double dy = z[j+7] - z[i+7];

					double c = std::pow(dx*dx + dy*dy, -1.5);

					out[i] += c * dx;
					out[i+1] += c * dy;
				}
			}

			for (int i = 0; i < 6; ++i) out[i + 6] = z[i];

			return out;
		};

		if (!read_y0) {
			const double _vx = 0.93240737 * 2 * std::numbers::pi,
				  		_vy = 0.86473146 * 2 * std::numbers::pi;

			y0 = pp::Vector<double>(12);
			
			// Start conditions from some python script at
			// https://github.com/spaceman-source/Three-Body_Problem/blob/main/Checiner-Montgomery%20Solution.py
			// (couldn't find them on wikipedia)
			y0[0] = 0.5*_vx; y0[1] = 0.5*_vy;
			y0[2] = -_vx; y0[3] = -_vy;
			y0[4] = 0.5*_vx; y0[5] = 0.5*_vy;

			y0[6] = -0.97000436; y0[7] = 0.24308753;
			y0[8] = 0.; y0[9] = 0.;
			y0[10] = 0.97000436; y0[11] = -0.24308753;
		}

	}
	else {
		std::cerr << "Choose a valid mode" << std::endl;
		return 1;
	}

	auto [x, y] = pp::driver(f, start, end, y0, initial_step, acc, eps, max_step);

	for (size_t i = 0; i < x.size(); ++i) {
		std::cout << x[i];
		pp::Vector<double> yi = y[i];
		for (size_t j = 0; j < yi.size; ++j) std::cout << " " << yi[j];
		std::cout << "\n";
	}
	std::cout << "\n" << std::endl;
	return 0;
}
