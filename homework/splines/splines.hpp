#pragma once
#include <cmath>
#include <functional>
#include <stdexcept>
#include <vector>

namespace pp {
	/*
	 * Returns index i such that x[[i]] <= z < x[[i+1]]. 
	 * Caller should ensure that x[[0]] <= z <= x[[x.size() - 1]]
	 */
	template <typename S>
	size_t binary_search(const std::vector<S>& x, const S& z) {
		size_t left = 0, right = x.size() - 1;
		
		while (right - left > 1) {
			size_t mid = (right + left) / 2;

			if (z < x[mid]) right = mid;
			else left = mid;
		}
		// left will be the last index satisfying x[left] <= z
		// and right = left + 1 will be the first index satisfying z < x[right]
		return left;
	}

	template <typename S>
	S lin_interp(const std::vector<S>& x, const std::vector<S>& y, const S& z) {
		if (x.front() <= z && z <= x.back()) {
			size_t i = binary_search(x, z);
			S dx = x[i+1] - x[i];
			S dy = y[i+1] - y[i];
			return y[i] + dy/dx * (z - x[i]);
		}
		else throw std::out_of_range("z not between first and last element of x");
	}

	template <typename S>
	S lin_interp_integral(const std::vector<S>& x, const std::vector<S>& y, const S& z) {
		if (x.front() <= z && z <= x.back()) {
			size_t i = binary_search(x, z);
			S out = (0.5 * (y[i+1] - y[i]) / (x[i+1] - x[i]) * (z - x[i]) + y[i]) * (z - x[i]); // integral from x[i] to z
			for (size_t j = 0; j < i; ++j) {
				out += 0.5 * (y[j+1] + y[j]) * (x[j+1] - x[j]); // analytical solution to integral between x[j] and x[j+1]
			}
			return out;
		}
		else throw std::out_of_range("z not between first and last element of x");
	}

	template <typename S>
	class LinearSpline {
	public:
		std::vector<S> x, y, p;
	
		static void compute_coefficients(const std::vector<S>& x, const std::vector<S>& y, std::vector<S>& out) {
			for (size_t i = 0; i < x.size() - 1; ++i) {
				out[i] = (y[i+1] - y[i]) / (x[i+1] - x[i]);
			}
		}

		LinearSpline(const std::vector<S>& x, const std::vector<S>& y) : x(x), y(y), p(x.size() - 1) {
			compute_coefficients(x,y,p);
		}

		S eval(const S& z) {
			size_t i = binary_search(x, z);
			return p[i] * (z - x[i]) + y[i];
		}

		S integral(const S& z) {
			size_t i = binary_search(x, z);
			S out = y[i] * (z - x[i]) + 0.5 * p[i] * std::pow((z - x[i]),2);
			for (size_t j = 0; j < i; ++j) {
				S dx = x[j+1] - x[j];
				out += y[j] * dx + 0.5 * p[j] * dx*dx;
			}
			return out;
		}

		S derivative(const S& z) {
			size_t i = binary_search(x, z);
			return p[i];
		}
	};

	template <typename S>
	class QuadraticSpline {
	public:
		std::vector<S> x, y, b, c;

		QuadraticSpline(const std::vector<S>& x, const std::vector<S>& y) : 
			x(x), y(y), b(x.size() - 1), c(x.size() - 1) {
			// make b contain p from linear spline
			LinearSpline<S>::compute_coefficients(x,y,b);

			c[0] = 0.;
			for (size_t i = 1; i < c.size(); ++i) {
				c[i] = (b[i] - b[i-1] - c[i-1] * (x[i] - x[i-1])) / (x[i+1] - x[i]);
			}
			c[c.size() - 1] *= 0.5;
			for (size_t i = c.size() - 1; i > 0; --i) {
				size_t j = i - 1;
				c[j] = (b[j+1] - b[j] - c[j+1] * (x[j+2] - x[j+1])) / (x[j+1] - x[j]);
			}

			for (size_t i = 0; i < b.size(); ++i) b[i] -= c[i] * (x[i+1] - x[i]);
		}

		S eval(const S& z) {
			size_t i = binary_search(x, z);
			S dx = z - x[i];
			return y[i] + b[i] * dx + c[i] * dx*dx;
		}

		S derivative(const S& z) {
			size_t i = binary_search(x, z);
			return b[i] + 2 * c[i] * (z - x[i]);
		}

		S integral(const S& z) {
			size_t i = binary_search(x, z);
			S dz = z - x[i];
			S out = y[i] * dz + 0.5 * b[i] * dz*dz + c[i] * dz*dz*dz / 3;
			for (size_t j = 0; j < i; ++j) {
				S dx = x[j+1] - x[j];
				out += y[j] * dx + 0.5 * b[j] * dx*dx + c[j] * dx*dx*dx / 3;
			}
			return out;
		}
	};
	
	template <typename S>
	std::function<S(const S&)> quadratic_spline(std::vector<S> x, std::vector<S> y) {
		std::vector<S> b(x.size() - 1), c(x.size() - 1);

		// make b contain p from linear spline
		LinearSpline<S>::compute_coefficients(x,y,b);

		c[0] = 0.;
		for (size_t i = 1; i < c.size(); ++i) {
			c[i] = (b[i] - b[i-1] - c[i-1] * (x[i] - x[i-1])) / (x[i+1] - x[i]);
		}
		c[c.size() - 1] *= 0.5;
		for (size_t i = c.size() - 1; i > 0; --i) {
			size_t j = i - 1;
			c[j] = (b[j+1] - b[j] - c[j+1] * (x[j+2] - x[j+1])) / (x[j+1] - x[j]);
		}

		for (size_t i = 0; i < b.size(); ++i) b[i] -= c[i] * (x[i+1] - x[i]);

		return [x = std::move(x), y = std::move(y), b = std::move(b), c = std::move(c)](const S& z) {
			size_t i = binary_search(x, z);
			S dx = z - x[i];
			return y[i] + b[i] * dx + c[i] * dx*dx;
		};
	}


	// IGNORE THIS IMPLEMENTATION. IT DOES NOT WORK.
	// template <typename S>
	// class CubicSpline {
	// public:
	// 	std::vector<S> x, y, b, c, d;
	// 	CubicSpline(const std::vector<S>& x, const std::vector<S>& y) :
	// 		x(x), y(y), b(x.size()), c(x.size() - 1), d(x.size() - 1) {
	//
	// 		size_t n = x.size();
	//
	// 		std::vector<S> p(n-1),h(n-1), D(n), Q(n-1), B(n);
	//
	// 		LinearSpline<S>::compute_coefficients(x,y,p);
	//
	// 		for (size_t i = 0; i < n-1; ++i) h[i] = x[i+1] - x[i];
	//
	// 		D[0] = 2.; D[n-1] = 2.;
	// 		for (size_t i = 1; i < n - 1; ++i) D[i] = 2. * h[i-1] / h[i] + 2.;
	//
	// 		Q[0] = 1.;
	// 		for (size_t i = 1; i < n - 1; ++i) Q[i] = h[i-1]/h[i];
	//
	// 		B[0] = 3. * p[0]; B[n-1] = 3.*p[n-2];
	// 		for (size_t i = 1; i < n - 2; ++i) B[i] = 3. * (p[i-1] + p[i] * h[i-1] / h[i]);
	//
	// 		std::vector<S> D_(n), B_(n);
	//
	// 		D_[0] = D[0];
	// 		B_[0] = B[0];
	// 		for (size_t i = 1; i < n; ++i) {
	// 			D_[i] = D[i] - Q[i-1] / D_[i-1];
	// 			B_[i] = B[i] - B_[i-1] / D_[i-1];
	// 		}
	//
	// 		b[n-1] = B_[n-1] / D_[n-1];
	// 		for (int i = n - 2; i <= 0; --i) {
	// 			b[i] = (B_[i] - Q[i] * b[i+1]) / D_[i];
	// 		}
	//
	// 		for (size_t i = 0; i < n - 1; ++i) {
	// 			c[i] = (-2. * b[i] - b[i+1] + 3.*p[i]) / h[i];
	// 			d[i] = (b[i] + b[i+1] - 2*p[i]) / (h[i] * h[i]);
	// 		}
	// 	}
	//
	// 	S eval(const S& z) {
	// 		size_t i = binary_search(x, z);
	// 		S dx = z - x[i];
	// 		return y[i] + b[i] * dx + c[i] * dx*dx + d[i] * dx*dx*dx;
	// 	}
	// }; CubicSpline
	
	

} // namespace pp
