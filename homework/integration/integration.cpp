#include "integration.hpp"
#include <cmath>
#include <cstdlib>
#include <functional>

namespace pp {
	constexpr double ONESIXTH = 1./6.;
	double integrate(const std::function<double(double)>& f, double a, double b, double acc, double eps) {
		double h = b - a;
		double f2 = f(a + 2*h * ONESIXTH);
		double f3 = f(a + 4*h * ONESIXTH);

		return rai(f, a, b, acc, eps, f2, f3);
	}

	double rai(const std::function<double(double)>& f, double a, double b, double acc, double eps, double f2, double f3) {
		double h = b-a;

		double f1 = f(a + h * ONESIXTH), f4 = f(a + 5*h * ONESIXTH);

		double Q = ONESIXTH * (2*f1 + f2 + f3 + 2*f4) * h;
		double q = 0.25 * (f1 + f2 + f3 + f4) * h;

		double err = std::abs(Q - q), tol = acc + eps * std::abs(Q);
		
		if (err < tol) return Q;
		else return rai(f, a, 0.5*(a+b), acc/std::sqrt(2.), eps, f1,f2) + 
			rai(f, 0.5*(a+b), b, acc/std::sqrt(2.), eps, f3, f4);
	}
}
