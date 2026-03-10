#include "../../lib/matrix.hpp"
#include <cstddef>

namespace pp {
	template <typename T, size_t ROWS = DYNAMIC, size_t COLS = DYNAMIC>
	class QR {
		Matrix<T,ROWS,COLS> Q;
		Matrix<T,COLS,COLS> R;

		QR(const Matrix<T, COLS, ROWS>& A) {
			Q = A;
			R = Matrix<T,COLS,COLS>(A.n_cols, A.n_cols);
			for (size_t i = 0; i < A.n_cols; ++i) {
				R[i,i] = norm(Q.col(i));
				Q.col(i) /= R[i,i];
				for (size_t j = 0; j < A.n_cols; ++j) {
					R[i,j] = dot(Q.col(i), Q.col(j));
					Q.col(j) -= Q[i] * R[i,j];
				}
			}
		}
	};
}
