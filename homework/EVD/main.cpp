#include "evd.hpp"
#include "../lib/matrix.hpp"
#include <iostream>

void test(size_t N) {
	pp::RandomMatrixGenerator RG(-1000., 1000.);

	pp::Matrix<double> A = RG.create_symmetric(N);

	pp::EVD evdA(A, false);

	pp::Matrix<double> D(N,N);
	for (size_t i = 0; i < N; ++i) D[i,i] = evdA.w[i];

	std::cout << "A =\n" << A << "\n\n";
	std::cout << "V =\n" << evdA.V << "\n\n";
	std::cout << "w =\n" << evdA.w << "\n\n";

	std::cout << "Vt A V =\n" << evdA.V.T() * A * evdA.V << "\n\n";
	std::cout << std::format("Vt A V equals D: {}\n\n", pp::mat_approx(evdA.V.T() * A * evdA.V, D, 0.0001, 0.0001));
	std::cout << "V D Vt =\n" << evdA.V * D * evdA.V.T() << "\n\n";
	std::cout << std::format("V D Vt equals A: {}\n\n", pp::mat_approx(evdA.V * D * evdA.V.T(), A, 0.00001, 0.00001));
	std::cout << "V Vt =\n" << evdA.V * evdA.V.T() << "\n\n";
	std::cout << "Vt V =\n" << evdA.V.T() * evdA.V << "\n\n";

	pp::EVD evdA_opt(A,true);
	std::cout << std::format("Optimized version gives same result: {}\n\n\n", pp::mat_approx(evdA.V, evdA_opt.V) && pp::mat_approx(evdA.w, evdA_opt.w));
}

int main(int argc, char** argv) {
	double rmax = 10;
	double dr = 0.3;
	bool test_output = false;
	bool e0_output = false;
	size_t n_wavefuncs = 0;
	size_t N = 10;
	bool timing_mode = false;
	bool use_optimized = true;

	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "--rmax" && ++i < argc) rmax = std::stod(argv[i]);
		else if (arg == "--dr" && ++i < argc) dr = std::stod(argv[i]);
		else if (arg == "--test") test_output = true;
		else if (arg == "--e0") e0_output = true;
		else if (arg == "--nwavefuncs" && ++i < argc) n_wavefuncs = std::stoi(argv[i]);
		else if (arg == "-N" && ++i < argc) N = std::stoi(argv[i]);
		else if (arg == "--time") timing_mode = true;
		else if (arg == "--use_unoptimized") use_optimized = false;
	}
	if (timing_mode) {
		pp::RandomMatrixGenerator RG(-1000., 1000.);
		pp::Matrix<double> A = RG.create_symmetric(N);
		pp::EVD<double> evdA = pp::EVD(A, use_optimized);
		return 0;
	}
	if (test_output) test(N);

	size_t n = rmax/dr - 1;
	pp::Matrix<double> H(n,n);
	pp::Vector<double> r(n);
	double reciprocal_dr2 = 1/(dr*dr);
	for (size_t i = 0; i < n; ++i) {
		r[i] = (i+1) * dr;
		H[i,i] = reciprocal_dr2 - 1/r[i];
	}
	for (size_t i = 0; i < n - 1; ++i) {
		H[i,i+1] = - 0.5 * reciprocal_dr2;
		H[i+1,i] = - 0.5 * reciprocal_dr2;
	}

	pp::EVD<double> evd(H, use_optimized);

	// std::cout << "H =\n" << H << "\n\n";
	// std::cout << "V =\n" << evd.V << "\n\n";
	// std::cerr << "Hydrogen eigenvalues:\n";
	// std::cerr << "w =\n" << evd.w << "\n\n";

	if (e0_output) std::cout << dr << " " << rmax << " " << evd.w[0] << "\n";

	for (size_t j = 0; j < n_wavefuncs; ++j) {
		for (size_t i = 0; i < n; ++i) {
			std::cout << r[i] << " " << evd.V[i,j]/std::sqrt(dr) << "\n";
		}
		std::cout << "\n\n";
	}
}
