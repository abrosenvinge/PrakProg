#include "../lib/matrix.hpp"
#include <functional>

namespace pp {
	void Jacobian( const std::function<Vector<double>(Vector<double>)>& f,
			Vector<double> x,
			const Vector<double>& fx,
			Matrix<double>& out);

	Vector<double> newton_qls(const std::function<Vector<double>(Vector<double>)>& f,
							Vector<double> x,
							double acc = 0.01,
							double lambda_min = 0.001,
							size_t max_iter = 100);

	Vector<double> newton( const std::function<Vector<double>(Vector<double>)>& f,
			Vector<double> x,
			double acc = 0.01,
			double lambda_min = 0.001,
			size_t max_iter = 100);

	std::function<pp::Vector<double>(double,pp::Vector<double>)> construct_ode(double E0); 

	double shooting_method_hydrogen(double rmin, double rmax, double E0,
			double acc = 0.0001, double eps = 0.0001);

} // namespace pp
