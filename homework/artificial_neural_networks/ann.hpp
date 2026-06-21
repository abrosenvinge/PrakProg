#pragma once
#include "../lib/matrix.hpp"

namespace pp {
	class ann {
	public:
		size_t n_hidden_neurons;
		Vector<double> p; // stored as {a0, b0, w0, a1, b1, w1, ...}
		ann(size_t n, pp::Vector<double> p0) : n_hidden_neurons(n), p(std::move(p0)) {
			// for (size_t i = 0; i < n_hidden_neurons; ++i) {
			// 	p[3*i] = -1 + (2.*i)/(n_hidden_neurons-1.);
			// 	p[3*i+1] = 6. / n_hidden_neurons;
			// 	p[3*i+2] = 8. / n_hidden_neurons;
			// }
		};

		virtual double activation(double x) = 0;
		virtual double activation_derivative(double x) = 0;
		virtual double activation_2nd_derivative(double x) = 0;
		virtual double activation_anti_derivative(double x) = 0;

		// computes the gradient of the network (wrt. p) and stores in out
		void gradient_p(double x, Vector<double>& out);

		// computes the Hessian of the network (wrt. p) and stores in out
		// Assumes that out = 0 except for 3x3 blocks on the diagonal
		void hessian_p(double x, Matrix<double>& out);

		double cost(const std::vector<double>& x, 
				const std::vector<double>& y);

		// computes C = sum((Fp(xi)-yi)^2) and its gradient and hessian wrt. p 
		// and stores the results in g and H. 
		// Assumes that H = 0 except for 3x3 blocks on the diagonal
		double cost_grad_hess_C(const std::vector<double>& x, 
				const std::vector<double>& y, 
				Vector<double>& g,
				Matrix<double>& H);

		double response(double x);
		double deriv(double x);
		double second_deriv(double x);
		double integral(double a, double b);
		void train(const std::vector<double>& x, const std::vector<double>& y);
	};

	class ann_Gaussian_wavelet : public ann {
	public:
		using ann::n_hidden_neurons, ann::p;
		ann_Gaussian_wavelet(size_t n, Vector<double> p0) : ann(n, std::move(p0)) { };

		double activation(double x) override;
		double activation_derivative(double x) override;
		double activation_2nd_derivative(double x) override;
		double activation_anti_derivative(double x) override;
	};

	class ann_Gaussian : public ann {
	public:
		using ann::n_hidden_neurons, ann::p;
		ann_Gaussian(size_t n, Vector<double> p0) : ann(n, std::move(p0)) { };

		double activation(double x) override;
		double activation_derivative(double x) override;
		double activation_2nd_derivative(double x) override;
		double activation_anti_derivative(double x) override;
	};

} //namespace pp
