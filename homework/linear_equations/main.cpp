#include "qr.hpp"
#include "../lib/matrix.hpp"
#include <cassert>
#include <iostream>

int main(int argc, char** argv) {
	size_t M = 15; // #rows
	size_t N = 10; // #cols
	bool time = false;
	
	for (int i = 0; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "-N" && ++i < argc) N = std::stoi(argv[i]);
		else if (arg == "-M" && ++i < argc) M = std::stoi(argv[i]);
		else if (arg == "--time") time = true;
	}

	pp::RandomMatrixGenerator RG(-100., 100.);

	pp::Matrix<double> A = RG.create(M,N);
	pp::QR<double> qr(A);
	if (time) return 0;

	pp::Vector<double> correctx = RG.create_vec(N);
	pp::Vector<double> b = A * correctx; // ensures that Ax = b has solution

	std::cout << "A =\n" << A << "\n\n";
	std::cout << "R =\n" << qr.R << "\n\n";
	std::cout << "Q =\n" << qr.Q << "\n\n";
	std::cout << "QR =\n" << qr.Q * qr.R << "\n\n";
	std::cout << "Q.T Q =\n" << qr.Q.T() * qr.Q << "\n\n";

	pp::Vector<double> x = qr.solve(b);
	std::cout << "correct x =\n" << correctx << "\n\n";
	std::cout << "x =\n" << x << "\n\n";
	std::cout << "b =\n" << b << "\n\n";
	std::cout << "Ax =\n" << A * x << "\n\n";

	pp::Matrix<double> B = RG.create(N,N);
	pp::QR<double> qrB(B);
	pp::Matrix<double> B_inv = qrB.inverse();
	std::cout << "B^-1 =\n" << B_inv << "\n\n";
	std::cout << "B^-1 B =\n" << B_inv * B << "\n\n";
	std::cout << "B B^-1 =\n" << B_inv * B << "\n\n";

	return 0;
}
