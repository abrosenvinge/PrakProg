#include "../lib/matrix.hpp"
#include <functional>

namespace pp {
	void grad_hess_central( const std::function<double(const pp::Vector<double>&)>& f,
			pp::Vector<double> x,
			pp::Vector<double>& g,
			pp::Matrix<double>& H);

	void gradient(const std::function<double(const pp::Vector<double>&)>& f,
			pp::Vector<double> x,
			pp::Vector<double>& out);

	void gradient_fd(const std::function<double(const pp::Vector<double>&)>& f,
			pp::Vector<double> x,
			double fx,
			pp::Vector<double>& out);

	void hessian(const std::function<double(const pp::Vector<double>&)>& f,
			pp::Vector<double> x,
			pp::Matrix<double>& out);

	void hessian_fd(const std::function<double(const pp::Vector<double>&)>& f,
			pp::Vector<double> x,
			const pp::Vector<double>& gfx,
			pp::Matrix<double>& out);

	struct MinimizationResult {
		double fx;
		pp::Vector<double> x;
		pp::Vector<double> gfx;
		pp::Matrix<double> Hfx;
		size_t n_iters;

		MinimizationResult(double fx, pp::Vector<double>& x, pp::Vector<double>& gfx, pp::Matrix<double>& Hfx, size_t n_iters) : 
			fx(fx), x(std::move(x)), gfx(std::move(gfx)), Hfx(std::move(Hfx)), n_iters(n_iters) {};
	};

	MinimizationResult min_newton(const std::function<double(const pp::Vector<double>&)>& f,
			pp::Vector<double> x,
			double acc = 0.0001,
			double min_lambda = 1./1024.,
			int max_iters = 10000);

	MinimizationResult min_newton_fd(const std::function<double(const pp::Vector<double>&)>& f,
			pp::Vector<double> x,
			double acc = 0.0001,
			double min_lambda = 1./1024.,
			int max_iters = 10000);

	MinimizationResult min_newton_central_unopt(const std::function<double(const pp::Vector<double>&)>& f,
			pp::Vector<double> x,
			double acc,
			double min_lambda,
			int max_iters);

	MinimizationResult leastsq_fit(const std::function<double(double, const pp::Vector<double>&)>& f,
			const std::vector<double>& x,
			const std::vector<double>& y,
			const std::vector<double>& sy,
			const pp::Vector<double>& p0,
			double acc = 0.0001,
			double min_lambda = 1./1024.,
			int max_iters = 10000);
}
