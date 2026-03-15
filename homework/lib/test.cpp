#include "matrix.hpp"
#include <iostream>

int main() {
	pp::Matrix<double> A(5,5), B(5,5);
	std::cout << typeid(A).name() << "\n";
	for (size_t i = 0; i < A.n_rows; ++i) {
		for (size_t j = 0; j < A.n_cols; ++j) {
			A[i,j] = i*i + j*j;
			B[i,j] = 2*i + j;
		}
	}

	std::cout << "A =\n" << A << "\n\n";
	std::cout << "B =\n" << B << "\n\n";

	std::cout << "A + B =\n" << A + B << "\n\n";

	std::cout << "AB = \n" << pp::mat_mult(A, B) << "\n\n";

	A *= 10.;
	std::cout << typeid(A).name() << "\n";
	std::cout << "A *= 10; A =\n" << A << "\n\n";

	pp::ColumnView<double> col = A.col(0);
	std::cout << "col = A.col(0); col =\n" << col << "\n\n";
	col[0] = 10;
	std::cout << "col[0] = 10; col =\n" << col << "\n\nA =\n" << A << "\n\n";

	pp::Matrix<double> C(5,6);

	A.col(0) = C.col(1);
	std::cout << "A.col(0) = C.col(1); A =\n" << A << "\n\n";

	std::cout << "A*A.col(1) =\n" << A.col(1) << "\n\n";

	pp::Vector<double> u(5), v(5);
	for (size_t i = 0; i < u.size; ++i) {
		u[i] = i;
		v[i] = i*i + 5;
	}

	std::cout << "u =\n" << u << "\n\n";
	std::cout << "v =\n" << v << "\n\n";
	std::cout << "A*u =\n" << A*u << "\n\n";
	std::cout << "dot(u,v) = " << dot(u,v) << "\n\n";
	std::cout << "norm(u) = " << norm(u) << "\n\n";
	u = u + v;
	std::cout << "u = u + v; u =\n" << u << "\n\n";
	u = A*u;
	std::cout << "u = A*u; u =\n" << u << "\n\n";

	// complex vector
	pp::VectorC<double> a(5), b(5);

	for (size_t i = 0; i < a.size; ++i) {
		a[i] = std::complex<double>(i,0);
		b[i] = std::complex<double>(0,i);
	}
	std::cout << "a =\n" << a << "\n\n";
	std::cout << "b =\n" << b << "\n\n";
	// std::cout << "A*b =\n" << A*b << "\n\n";
	std::cout << "dot(a,b) = " << dot(a,b) << "\n\n";
	std::cout << "norm(a) = " << norm(a) << "\n\n";


	pp::TransposeView<double> AT = A.T();
	std::cout << "A.T() =\n" << AT << "\n\n";
	std::cout << "A.T() * B\n" << AT * B << "\n\n";

	std::cout << "mat_approx((B.T() * A.T()).T(), A*B) = " << mat_approx((B.T() * A.T()).T(), A*B) << "\n\n";

	A.T() = B;
	std::cout << "A.T() = B; A = \n" << A << "\n\n";
	std::cout << "mat_approx(A, B.T()) = " << mat_approx(A, B.T()) << "\n\n";

	pp::Matrix<double> I5 = pp::Matrix<double>::ident(5);
	std::cout << "I5 =\n" << I5 << "\n\n";

	std::cout << A.is_utriangular() << "\n\n";

	pp::Matrix<double> D(7,5);
	std::cout << D.n_cols << "\n";
	std::cout << D.n_rows << "\n";
	for (size_t j = 0; j < D.n_cols; ++j) 
		for (size_t i = 0; i < D.n_rows; ++i) D[i,j] = i + j;

	std::cout << "D =\n" << D << "\n\n";
	D.col(0) -= D.col(1);
	std::cout << "D.col(0) -= D.col(1); D =\n" << D << "\n\n";
}
