#include <iostream>
#include <vector>
#include "vec.hpp"

template <typename T> void print_vec(std::vector<T> v) {
	for (T vi : v) std::cout << vi << " ";
	std::cout << "\n";
}

int main () {
	std::vector<double> v = {1,2,3};

	auto &u = v;

	print_vec(u);
	print_vec(v);

	// nothing happens (ui copied)
	for (auto ui : u) ui = 6;
	print_vec(u);

	// changed (vi referenced)
	for (auto &vi : v) vi = 6;
	print_vec(v);

	pp::vec3 a;
	a.x = 10.0;
	a.y = 6.0;
	a.z = 3.0;

	std::cout << a.x << " " << a.y << " " << a.z << std::endl;

	return 0;
}
