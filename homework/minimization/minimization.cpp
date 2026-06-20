#include "minimization.hpp"
#include "../lib/matrix.hpp"
#include "../linear_equations/qr.hpp"
#include <cmath>
#include <functional>

namespace pp {
	void gradient(const std::function<double(const pp::Vector<double>&)>& f,
			pp::Vector<double> x,
			pp::Vector<double>& out) 
	{
		pp::Vector<double> x2(x);
		double dx;
		for(size_t i = 0; i < x.size; ++i) {
			dx = (1 + std::abs(x[i])) * std::pow(2., -17.3333333); // epsilon^(1/3)
			x[i] += dx;
			x2[i] -= dx;
			out[i] = (f(x) - f(x2)) / (2*dx);
			x[i] -= dx;
			x2[i] += dx;
		}
	}
	void gradient_fd(const std::function<double(const pp::Vector<double>&)>& f,
			pp::Vector<double> x,
			double fx,
			pp::Vector<double>& out) 
	{
		double dx;
		for(size_t i = 0; i < x.size; ++i) {
			dx = (1 + std::abs(x[i])) * std::pow(2, -26);
			x[i] += dx;
			out[i] = (f(x) - fx) / dx;
			x[i] -= dx;
		}
	}

	void hessian(const std::function<double(const pp::Vector<double>&)>& f,
			pp::Vector<double> x,
			pp::Matrix<double>& out)
	{
		double dxj, dxi;
		for(size_t j = 0; j < x.size; ++j) {
			dxj = (1 + std::abs(x[j])) * std::pow(2., -17.3333333);
			for (size_t i = 0; i < x.size; ++i) {
				out[i,j] = 0.;
				for (int c = -1; c <= 1; c += 2) {
					x[j] += c * dxj;
					dxi = (1 + std::abs(x[i])) * std::pow(2., -17.3333333);
					for (int d = -1; d <= 1; d += 2) {
						x[i] += d * dxi;
						out[i,j] += c*d*f(x) / (4*dxj*dxi);
						x[i] -= d * dxi;
					}
					x[j] -= c * dxj;
				}
			}
		}
	}

	void hessian_fd(const std::function<double(const pp::Vector<double>&)>& f,
			pp::Vector<double> x,
			const pp::Vector<double>& gfx,
			pp::Matrix<double>& out)
	{
		double dx;
		pp::Vector<double> g(gfx.size);
		for(size_t j = 0; j < x.size; ++j) {
			dx = (1 + std::abs(x[j])) * std::pow(2, -13);
			x[j] += dx;
			gradient_fd(f, x, f(x), g);
			for(size_t i = 0; i < g.size; ++i) out[i,j] = (g[i] - gfx[i])/dx;
			x[j] -= dx;
		}
	}

	MinimizationResult min_newton_fd(const std::function<double(const pp::Vector<double>&)>& f,
			pp::Vector<double> x,
			double acc,
			double min_lambda,
			int max_iters)
	{
		double fx = f(x);
		pp::Vector<double> g(x.size);
		gradient_fd(f, x, fx, g);

		pp::Matrix<double> H(x.size,x.size);
		hessian_fd(f, x, g, H);

		int iters = 0;
		
		while (norm(g) >= acc && iters < max_iters) {
			for (size_t i = 0; i < H.n_rows; ++i) H[i,i] += 1e-6;

			pp::QR<double> qrH(H);
			pp::Vector<double> dx = qrH.solve(-g);

			double lambda = 1.;
			pp::Vector<double> new_x = x + dx;

			while (f(new_x) >= fx && lambda >= min_lambda) {
				lambda *= 0.5;
				dx *= 0.5;
				new_x -= dx;
			}

			x = new_x;
			fx = f(new_x); // this evalution can be optimized away, since it was evaluated in the loop
			gradient_fd(f, x, fx, g);
			hessian_fd(f, x, g, H);

			iters++;
		}

		return MinimizationResult(fx, x, g, H, iters);
	}

	MinimizationResult min_newton(const std::function<double(const pp::Vector<double>&)>& f,
			pp::Vector<double> x,
			double acc,
			double min_lambda,
			int max_iters)
	{
		double fx = f(x);
		pp::Vector<double> g(x.size);
		gradient(f, x, g);

		pp::Matrix<double> H(x.size,x.size);
		hessian(f, x, H);

		int iters = 0;
		
		while (norm(g) >= acc && iters < max_iters) {
			for (size_t i = 0; i < H.n_rows; ++i) H[i,i] += 1e-6;

			pp::QR<double> qrH(H);
			pp::Vector<double> dx = qrH.solve(-g);

			double lambda = 1.;
			pp::Vector<double> new_x = x + dx;

			while (f(new_x) >= fx && lambda >= min_lambda) {
				lambda *= 0.5;
				dx *= 0.5;
				new_x -= dx;
			}

			x = new_x;
			fx = f(new_x); // this evalution can be optimized away, since it was evaluated in the loop
			gradient(f, x, g);
			hessian(f, x, H);

			iters++;
		}

		return MinimizationResult(fx, x, g, H, iters);
	}

	MinimizationResult leastsq_fit(const std::function<double(double, const pp::Vector<double>&)>& f,
			const std::vector<double>& x,
			const std::vector<double>& y,
			const std::vector<double>& sy,
			const pp::Vector<double>& p0,
			double acc,
			double min_lambda,
			int max_iters) 
	{
		auto cost = [&f, &x, &y, &sy](const pp::Vector<double>& p) {
			double chi2 = 0;
			for (size_t i = 0; i < x.size(); ++i) {
				double r = (f(x[i], p) - y[i])/sy[i];
				chi2 += r*r;
			}
			return chi2;
		};
		return min_newton(cost, p0, acc, min_lambda, max_iters);
	}
}
