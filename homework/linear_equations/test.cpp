#include <iostream>
#include "../lib/matrix.hpp"
#include "qr.hpp"

bool print_if_false(bool expr, std::string test_name, size_t i, size_t j) {
	if (!expr) std::cout << std::format("Test failed for {}x{} matrix: {}\n", i,j,test_name);
	return expr;
}

int main() {
	pp::RandomMatrixGenerator generator(-1000., 1000.);

	bool passed_all = true;
	for (size_t j = 5; j < 100; j += 5) {
		for (size_t i = j; i < 10 * j; i += j) {
			bool passed = true;

			pp::Matrix<double> A = generator.create(i,j);
			pp::Vector<double> correctx = generator.create_vec(j);
			pp::Vector<double> b = A * correctx;

			pp::QR<double> qr(A);

			passed &= print_if_false(qr.R.is_utriangular(), "R Upper triangular", i, j);
			passed &= print_if_false(pp::mat_approx(qr.Q * qr.R, A), "QR=A", i, j);
			passed &= print_if_false(pp::mat_approx(qr.Q.T() * qr.Q, pp::Matrix<double>::ident(qr.Q.n_cols)), "Q orthogonal", i, j);

			pp::Vector<double> x = qr.solve(b);
			passed &= print_if_false(pp::mat_approx(x, correctx, 1e-6, 1e-6), "x correct solution", i, j);
			passed &= print_if_false(pp::mat_approx(A*x, b), "Ax = b", i, j);

			if (i == j && !pp::approx(qr.det(),0.)) { 
				pp::Matrix<double> A_inv = qr.inverse();
				pp::Matrix<double> I = pp::Matrix<double>::ident(i);
				passed &= print_if_false(pp::mat_approx(A * A_inv, I), "AA_inv = I", i, j);
				passed &= print_if_false(pp::mat_approx(A_inv * A, I, 1e-6, 1e-6), "A_invA = I", i, j);
			}

			if (!passed)
				std::cerr << std::format("Some tests failed for {}x{} matrix\n", i, j); 
			passed_all &= passed;
		}
	}
	if (passed_all) std::cout << "All tests passed";
}
