#include "vec.hpp"
#include <format>
#include <iostream>
#include <complex>

int main() {
	vec::vec<double> v(1,2,3);
	vec::vec<double> u(4,5,6);

	std::cout << "v = " << v << "\n";
	std::cout << "u = " << u << "\n";
	std::cout << "-v = " << -v << "\n";

	v += u;
	v *= 2;
	std::cout << "v += u; v *= 2; v = " << v << "\n";

	v /= 2;
	std::cout << "v /= 2; v = " << v << "\n";

	std::cout << "u + v = " << u + v << "\n";

	std::cout << "5*v = " << 5.*v << "\n";
	std::cout << "v*5 = " << v*5. << "\n";
	std::cout << "v/5 = " << v/5. << "\n";

	std::cout << "v.norm() = " << v.norm() << "\n";
	std::cout << "dot(u,v) = " << vec::dot(u,v) << "\n";

	vec::vec cross_test = vec::cross(vec::vec(1,0,0), vec::vec(0,1,0));
	std::cout << "cross(u,v) = " << cross_test << "\n";

	std::cout << std::format("u == v: {}\n", vec::approx(u,v)); //should be false
	std::cout << std::format("approx(u,u + (1e-9, 1e-9, 1e-9)): {}", 
			vec::approx(u, u + vec::vec(1e-9, 1e-9, 1e-9))) << "\n";

	const std::complex<double> Im = std::complex(0.,1.);
	const std::complex<double> Re = std::complex(1.,0.);
	vec::vec<std::complex<double>> complex_v(Re, Im, Re + Im);
	std::cout << complex_v << "\n";
	complex_v *= 5;
	std::cout << complex_v << "\n";
}
