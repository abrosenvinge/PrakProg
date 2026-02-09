#include "vec.hpp"
#include <iostream>

int main() {
	vec::vec v(1,2,3);
	vec::vec u(4,5,6);

	std::cout << v << "\n";
	std::cout << -v << "\n";

	v += u;
	v *= 2;
	std::cout << v << "\n";

	v /= 2;
	std::cout << v << "\n";

	std::cout << u + v << "\n";

	std::cout << 5*v << "\n";
	std::cout << v*5 << "\n";
	std::cout << v/5 << "\n";

	std::cout << v.norm() << "\n";
	std::cout << u*v << "\n";
}
