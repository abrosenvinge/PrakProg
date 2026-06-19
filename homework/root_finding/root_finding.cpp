#include "../lib/matrix.hpp"
#include <functional>
#include <iostream>
#include "../linear_equations/qr.hpp"
#include "../ODE/ode.hpp"

namespace pp {
	void Jacobian(const std::function<Vector<double>(Vector<double>)>& f,
				Vector<double> x,
				const Vector<double>& fx,
				Matrix<double>& out) 
	{
		size_t n = x.size, m = fx.size;
		for (size_t j = 0; j < n; ++j) {
			double dx = std::max(std::abs(x[j]),1.) * std::pow(2,-26);
			x[j] += dx;
			Vector<double> df = f(x) - fx;			
			for (size_t i = 0; i < m; ++i) out[i,j] = df[i] / dx;
			x[j] -= dx;
		}
	}

	Vector<double> newton(const std::function<Vector<double>(Vector<double>)>& f,
							Vector<double> x,
							double acc = 0.01,
							double lambda_min = 0.001,
							size_t max_iter = 100)
	{
		Vector<double> fx = f(x);
		Matrix<double> jac(fx.size, x.size);

		double norm_fx = norm(fx);
		size_t iters = 0;

		while (norm_fx >= acc && iters <= max_iter) {
			Jacobian(f, x, fx, jac);

			QR<double> qr(jac);
			Vector<double> dx = qr.solve(-fx);
			
			double lambda = 1.;

			x += dx;
			fx = f(x);

			double new_norm_fx = norm(fx);
			while (new_norm_fx > (1. - 0.5*lambda)*norm_fx && lambda >= lambda_min) {
				lambda *= 0.5;
				dx *= 0.5;
				x -= dx;
				fx = f(x);
				new_norm_fx = norm(fx);
			}
			norm_fx = new_norm_fx;
			iters++;
		}
		return x;
	}

	std::vector<Vector<double>> newton(
							const std::function<Vector<double>(Vector<double>)>& f,
							const std::vector<Vector<double>>& x,
							double acc,
							double lambda_min,
							size_t max_iter) 
	{
		std::vector<Vector<double>> out;
		out.reserve(x.size());
		for (const Vector<double>& x0 : x) 
			out.push_back(newton(f, x0, acc, lambda_min, max_iter));
		return out;
	}

	std::function<pp::Vector<double>(double,pp::Vector<double>)> construct_ode(double E0) {
		return [E0](double r, pp::Vector<double> y) {
			pp::Vector<double> out(y.size);
			out[0] = y[1];
			out[1] = -2.*(E0+1./r)*y[0];
			return out;
		};
	}

	double shooting_method_hydrogen(double rmin, double rmax, double E0,
			double acc, double eps) {
		pp::Vector<double> y0 {rmin-rmin*rmin, 1.-2.*rmin};
		std::function<pp::Vector<double>(pp::Vector<double>)> M =
			[&y0, rmin, rmax,acc,eps](pp::Vector<double> E) {
				double E0 = E[0];
				auto ode = construct_ode(E0);
				pp::Vector<double> res =  pp::endpoint_driver(ode, rmin, rmax, y0, 0.125, acc,eps);
				return pp::Vector{res[0]};
			};

		double E =  pp::newton(M, pp::Vector<double>{E0}, acc,0.0001,10000)[0];
		return E;
	}

} // namespace pp
