#pragma once
#include "../lib/matrix.hpp"
#include <cmath>
#include <stdexcept>

namespace pp {

	template <typename S>
	class EVD {
	public:
		class UpperTriangularHelper {
		public:
			std::vector<S> data;
			size_t n;

			UpperTriangularHelper(const MatrixBase<S>& A) : data((A.n_cols * (A.n_cols + 1)) / 2), n(A.n_rows) {
				for (size_t j = 0; j < n; ++j) {
					for (size_t i = 0; i <= j; ++i) (*this)[i,j] = A[i,j];
				}
			}

			S& operator[](size_t i, size_t j) {
				return data[(j * (j + 1)) / 2 + i]; // we may as well only store the upper triangle
			}
		};

		static void jacobi_rot(UpperTriangularHelper& A, size_t p, size_t q, double theta) {
			if (!(p < q)) throw std::invalid_argument("p must be less than q");
			double s(std::sin(theta)), c(std::cos(theta));

			// first update App, Apq, Aqq, since these must be updated in correct order
			S App = A[p,p], Apq = A[p,q], Aqq = A[q,q];

			A[q,q] = s*s*App + 2*s*c*Apq + c*c*Aqq;
			A[p,p] = c*c*App - 2*s*c*Apq + s*s*Aqq;
			A[p,q] = s*c*(App - Aqq) + (c*c - s*s) * Apq;

			// Update the part of the matrix between column and row p and q,
			// since these require correct order since they depend on each other.
			// The matrix is updated from both sides at once to avoid allocating additional
			// memory.
			for (size_t k = 1; k < (q - p + 1)/2; ++k) {
				size_t i = p + k;
				size_t j = q - k;

				S Api = A[p,i], Apj = A[p,j], Aiq = A[i,q], Ajq = A[j,q];

				A[i,q] = s*Api + c*Aiq;
				A[j,q] = s*Apj + c*Ajq;
				
				A[p,i] = c*Api - s*Aiq;
				A[p,j] = c*Apj - s*Ajq;
			}

			// if there is an odd number of indices between p and q the middle must 
			// be updated seperately. (It could be done be extending the above loop, but 
			// since this will do twice as many lookups and assignments, the below is 
			// slightly faster according to my testing.)
			if ((p - q) % 2 == 0) {
				size_t i = (p + q) / 2;
				S Api = A[p,i], Aiq = A[i,q];

				A[p,i] = c*Api - s*Aiq;
				A[i,q] = s*Api + c*Aiq;
			}

			// update the remaining p rows of column p and q
			for (size_t i = 0; i < p; ++i) {
				S Aip = A[i,p];
				S Aiq = A[i,q];
				A[i,q] = s * Aip + c * Aiq;
				A[i,p] = c * Aip - s * Aiq;
			}

			// update the remaining n - q columns of row p and q
			for (size_t j = q + 1; j < A.n; ++j) {
				S Apj = A[p,j];
				S Aqj = A[q,j];
				A[p,j] = c*Apj - s*Aqj;
				A[q,j] = c*Aqj + s*Apj;
			}
		}
	
	void construct_unoptimized(Matrix<S> A) {
		size_t n = A.n_rows;
		V = Matrix<S>::ident(n);

		bool changed = true;
		while (changed) {
			changed = false;
			for (size_t q = 0; q < n; ++q) {
				for (size_t p = 0; p < q; ++p) {
					S App = A[p,p], Aqq = A[q,q], Apq = A[p,q]; // only changing diagonal elements
					double theta = 0.5 * std::atan2(2*Apq, Aqq - App);

					double c = std::cos(theta), s = std::sin(theta);
					S new_App = c*c*App-2*s*c*Apq+s*s*Aqq;
					S new_Aqq = s*s*App+2*s*c*Apq+c*c*Aqq;

					if (new_App != App || new_Aqq != Aqq) {
						changed = true;
						timesJ(A, p, q, theta);
						Jtimes(A, p, q,-theta);
						timesJ(V, p, q, theta);
					}
				}
			}
		}

		for (size_t i = 0; i < n; ++i) w[i] = A[i,i];
	}

	void construct_optimized(const Matrix<S>& B) {
		UpperTriangularHelper A(B);
		size_t n = A.n;
		V = Matrix<S>::ident(n);

		bool changed = true;
		while (changed) {
			changed = false;
			for (size_t q = 0; q < n; ++q) {
				for (size_t p = 0; p < q; ++p) {
					S App = A[p,p], Aqq = A[q,q], Apq = A[p,q]; // only changing diagonal elements
					double theta = 0.5 * std::atan2(2*Apq, Aqq - App);

					double c = std::cos(theta), s = std::sin(theta);
					S new_App = c*c*App-2*s*c*Apq+s*s*Aqq;
					S new_Aqq = s*s*App+2*s*c*Apq+c*c*Aqq;

					if (new_App != App || new_Aqq != Aqq) {
						changed = true;
						jacobi_rot(A, p, q, theta);
						timesJ(V, p, q, theta);
					}
				}
			}
		}

		for (size_t i = 0; i < n; ++i) w[i] = A[i,i];
	}

	public:
		static void timesJ(Matrix<S>& A, size_t p, size_t q, double theta) {
			double s(std::sin(theta)), c(std::cos(theta));

			for (size_t i = 0; i < A.n_rows; ++i) {
				S Aip = A[i,p];
				S Aiq = A[i,q];
				A[i, p] = c * Aip - s * Aiq;
				A[i, q] = s * Aip + c * Aiq;
			}
		}
		static void Jtimes(Matrix<S>& A, size_t p, size_t q, double theta) {
			double s(std::sin(theta)), c(std::cos(theta));

			for (size_t j = 0; j < A.n_cols; ++j) {
				S Apj = A[p,j];
				S Aqj = A[q,j];
				A[p,j] = c*Apj + s*Aqj;
				A[q,j] = c*Aqj - s*Apj;
			}
		}

		Matrix<S> V;
		Vector<S> w;

		EVD(const Matrix<S>& A, bool use_optimized = true) : w(A.n_rows) {
			if (!A.is_square()) throw std::invalid_argument("EVD of non-square matrix");

			if (use_optimized) construct_optimized(A);
			else construct_unoptimized(A);
		}
	};
}
