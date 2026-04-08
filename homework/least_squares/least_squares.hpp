#pragma once
#include "../lib/matrix.hpp"
#include <functional>
#include <vector>
#include "../linear_equations/qr.hpp"

namespace pp {
	template <typename S>
	std::tuple<Vector<S>, Matrix<S>> lsfit(
			const std::vector<std::function<S(S)>>& fs,
			const VectorBase<S>& x,
			const VectorBase<S>& y,
			const VectorBase<S>& dy
			)
	{
		size_t m = x.size, n = fs.size();
		Matrix<S> A(m,n);
		Vector<S> b(m);

		for (size_t k = 0; k < n; ++k) {
			for (size_t i = 0; i < m; ++i) {
				A[i,k] = fs[k](x[i]) / dy[i];
			}
		}
		for (size_t i = 0; i < m; ++i) {
			b[i] = y[i] / dy[i];
		}

		QR<S> qrA(A);

		Vector<S> c = qrA.solve(b);

		const Matrix<S>& R = qrA.R;
		Matrix<S> Rinv(qrA.R.n_rows, qrA.R.n_cols);
		
		// does back substitution to solve R x_j = e_j, where x_j is the j'th column of R^-1
		// finding the inverse is usually slow, but R is likely a very small matrix
		for (size_t j = 0; j < Rinv.n_cols; ++j) {
			// because e_j[i] is 0 for i > j then x_j[i] = 0 for i > j.
			size_t i = j;
			Rinv[i,j] = 1./R[i,i];
			
			while (i > 0) {
				i--;
				double Rinvij = 0.;
				// only need to iterate up to j
				for (size_t k = i; k <= j; ++k) Rinvij -= R[i,k] * Rinv[k,j];
				Rinv[i,j] = Rinvij / R[i,i];
			}
		}

		Matrix<S> covariance_matrix = Rinv * (Rinv.T());
		
		return {c, covariance_matrix};
	}
}
