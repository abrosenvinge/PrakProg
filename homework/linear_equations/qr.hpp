#pragma once
#include "../lib/matrix.hpp"
#include <cstddef>
#include <stdexcept>

namespace pp {
	/*
	 * Solves the linear system Ax = b by back substition,
	 * where A is an upper triangular matrix.
	 */
	template <typename T>
	Vector<T> back_substitution(const MatrixBase<T>& A, const VectorBase<T>& b) {
		size_t n = b.size;
		Vector<T> x(n);
		for (size_t i = n; i > 0; --i) {
			size_t k = i - 1;
			x[k] = b[k];
			for (size_t j = i; j < n; ++j) {
				x[k] -= A[k,j] * x[j];
			}
			x[k] /= A[k,k];
		}
		return x;
	}

	template <typename T>
	class QR {
	public:
		Matrix<T> Q;
		Matrix<T> R;

		QR(const MatrixBase<T>& A) : Q(A), R(A.n_cols, A.n_cols){
			for (size_t i = 0; i < A.n_cols; ++i) {
				R[i,i] = norm(Q.col(i));
				Q.col(i) /= R[i,i];
				for (size_t j = i + 1; j < A.n_cols; ++j) {
					R[i,j] = dot(Q.col(i), Q.col(j));
					Q.col(j) -= Q.col(i) * R[i,j];
				}
			}
		}

		Vector<T> solve(const VectorBase<T>& b) {
			return back_substitution(R, Q.T() * b);
		}

		T det() {
			T result(0.);
			for (size_t i = 0; i < R.n_rows; ++i) {
				result += R[i,i];
			}
			return result;
		}
		Matrix<T> inverse() {
			if (!Q.is_square()) throw std::invalid_argument("Non-square matrix is not invertible");
			Matrix<T> out = Q.T_copy();
			for (size_t i = 0; i < Q.n_rows; ++i) {
				out.col(i) = back_substitution(R, out.col(i));
			}
			return out;
		}
	};
}
