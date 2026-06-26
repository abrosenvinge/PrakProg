#include "ann.hpp"
#include "../lib/matrix.hpp"
#include <cmath>
#include <numbers>
#include "../linear_equations/qr.hpp"
#include "../minimization/minimization.hpp"
#include "../integration/integration.hpp"

namespace pp {
	double ann::response(double x, const pp::Vector<double>& ptest) {
		double out = 0;
		for (size_t i = 0; i < n_hidden_neurons; ++i) {
			double ai = ptest[3*i], bi = ptest[3*i+1], wi = ptest[3*i+2];
			out += activation((x-ai)/bi)*wi;
		}
		return out;
	}
	double ann::response(double x) { return response(x,p); }

	double ann::deriv(double x, const pp::Vector<double>& ptest) {
		double out = 0;
		for (size_t i = 0; i < n_hidden_neurons; ++i) {
			double ai = ptest[3*i], bi = ptest[3*i+1], wi = ptest[3*i+2];
			out += activation_derivative((x-ai)/bi)*wi / bi;
		}
		return out;
	}
	double ann::deriv(double x) { return deriv(x,p); }

	double ann::second_deriv(double x, const pp::Vector<double>& ptest) {
		double out = 0;
		for (size_t i = 0; i < n_hidden_neurons; ++i) {
			double ai = ptest[3*i], bi = ptest[3*i+1], wi = ptest[3*i+2];
			out += activation_2nd_derivative((x-ai)/bi)*wi / (bi*bi);
		}
		return out;

	}
	double ann::second_deriv(double x) { return second_deriv(x,p); }

	double ann::integral(double a, double b, const pp::Vector<double>& ptest) {
		double out = 0;
		for (size_t i = 0; i < n_hidden_neurons; ++i) {
			double ai = ptest[3*i], bi = ptest[3*i+1], wi = ptest[3*i+2];
			out += (activation_anti_derivative((b-ai)/bi) - 
					activation_anti_derivative((a-ai)/bi))*wi*bi;
		}
		return out;
	}
	double ann::integral(double a, double b) { return integral(a,b,p); }

	void ann::gradient_p(double x, Vector<double>& out) {
		for (size_t i = 0; i < n_hidden_neurons; ++i) {
			size_t k = 3*i;
			double ai = p[k], bi = p[k+1], wi = p[k+2];
			double deriv = activation_derivative((x-ai)/bi);
			out[k] = - wi / bi * deriv;
			out[k+1] = -(x-ai)*wi/(bi*bi) * deriv;
			out[k+2] = activation((x-ai)/bi);
		}
	}

	void ann::hessian_p(double x, Matrix<double>& out) {
		for (size_t i = 0; i < n_hidden_neurons; ++i) {
			size_t k = 3*i;
			double a = p[k], b = p[k+1], w = p[k+2];
			double deriv = activation_derivative((x-a)/b);
			double deriv2 = activation_2nd_derivative((x-a)/b);

			out[k,k] = w * deriv2 / (b*b);
			double ab = w*deriv/(b*b) + w*(x-a)*deriv2/(b*b*b);
			out[k,k+1] = ab; out[k+1,k] = ab;
			double aw = -deriv / b;
			out[k+2,k] = aw; out[k,k+2] = aw;

			out[k+1,k+1] = 2*w*(x-a)*deriv / (b*b*b) + w*(x-a)*(x-a)*deriv2 / (b*b*b*b);

			double bw = -(x-a)*deriv/(b*b);
			out[k+1,k+2] = bw; out[k+2,k+1] = bw;

			out[k+2,k+2] = 0.;
		}
	}
	double ann::cost(const std::vector<double>& x, 
			const std::vector<double>& y,
			const pp::Vector<double>& ptest) {
		double out = 0;
		for (size_t i = 0; i < x.size(); ++i) {
			out += std::pow(response(x[i],ptest) - y[i],2);
		}
		return out;
	}

	double ann::cost(const std::vector<double>& x, 
			const std::vector<double>& y) {
		return cost(x,y,p);
	}

	double ann::cost_grad_hess_C(const std::vector<double>& x, 
			const std::vector<double>& y, 
			Vector<double>& gout,
			Matrix<double>& Hout) 
	{	
		double Cout = 0;
		pp::Vector<double> g(gout.size);
		pp::Matrix<double> H(Hout.n_rows, Hout.n_cols);
		for (size_t j = 0; j < gout.size; ++j) {
			for (size_t i = 0; i < gout.size; ++i) {
				Hout[i,j] = 0.;
			}
			gout[j] = 0.;
		}
		for (size_t n = 0; n < x.size(); ++n) {
			double xn = x[n], yn = y[n];
			double fx = response(xn);
			gradient_p(xn, g);
			hessian_p(xn, H);

			for (size_t j = 0; j < p.size; ++j) {
				for (size_t i = 0; i < p.size; ++i) {
					Hout[i,j] += 2*(fx-yn)*H[i,j] + 2*g[i]*g[j];
				}
				gout[j] += 2 * (fx - yn) * g[j];
			}
			Cout += (fx - yn) * (fx - yn);
		}
		return Cout;
	}

	void ann::train_lstsq(const std::vector<double>& x, const std::vector<double>& y) {
		pp::Vector<double> g(p.size);
		pp::Matrix<double> H(p.size,p.size);

		double C = cost_grad_hess_C(x, y, g, H);

		int iters = 0, max_iters = 10000;
		double acc = 0.0001, min_lambda = 0.00001;

		while (norm(g) >= acc && iters < max_iters) {
			for (size_t i = 0; i < H.n_rows; ++i) H[i,i] += 1e-6;

			pp::QR<double> qrH(H);
			pp::Vector<double> dp = qrH.solve(-g);

			// std::cerr << H << "\n\n";

			double lambda = 1.;
			p += dp;

			while (cost(x,y) >= C && lambda >= min_lambda) {
				lambda *= 0.5;
				dp *= 0.5;
				p -= dp;
			}

			C = cost_grad_hess_C(x,y,g,H);

			iters++;
		}
		// auto C = [this,&x, &y](const pp::Vector<double>& ptest) { return cost(x,y,ptest); };
		// MinimizationResult min_res = min_newton(C, p);
		// p = min_res.x;
	}

	// Trains the ann to approximate the solution, y(x), to the differential equation Phi(y'',y',y,x) = 0
	// satisfying y(c) = yc and y'(c) = dyc on the interval [a,b]
	void ann::train_diffeq(const std::function<double(double, double, double, double)> Phi, 
			double a, 
			double b,
			double c,
			double yc,
			double dyc,
			double alpha,
			double beta) 
	{
		auto C = [this, a, b, c, yc, dyc, alpha, beta, &Phi](const pp::Vector<double>& ptest) {
			auto integrand = [this, &ptest, &Phi](double x) {
				double P = Phi(second_deriv(x,ptest), deriv(x,ptest), response(x,ptest), x);
				return P*P;
			};
			auto [I,_] = integrate(integrand, a, b);
			double yc_err = (response(c,ptest)-yc);
			double dyc_err = (deriv(c,ptest)-dyc);
			double boundary_term = alpha * yc_err * yc_err + beta * dyc_err * dyc_err;
			return I + boundary_term;
		};
		MinimizationResult min_res = min_newton(C, p, 0.001, 0.001, 1000);
		p = min_res.x;
	}
	
	double ann_Gaussian_wavelet::activation(double x) {
		return x * std::exp(-x*x);
	}

	double ann_Gaussian_wavelet::activation_derivative(double x) {
		return (1 - 2*x*x) * std::exp(-x*x);
	}

	double ann_Gaussian_wavelet::activation_2nd_derivative(double x) {
		return 2 * std::exp(-x*x) * x * (2*x*x-3);
	}

	double ann_Gaussian_wavelet::activation_anti_derivative(double x) {
		return -0.5*std::exp(-x*x);
	}

	double ann_Gaussian::activation(double x) {
		return std::exp(-x*x);
	}

	double ann_Gaussian::activation_derivative(double x) {
		return 2 * x * std::exp(-x*x);
	}

	double ann_Gaussian::activation_2nd_derivative(double x) {
		return std::exp(-x*x) * (4*x*x - 2);
	}

	double ann_Gaussian::activation_anti_derivative(double x) {
		return 0.5 * std::sqrt(std::numbers::pi) * std::erf(x);
	}

}
