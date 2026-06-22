#include "bilinear.hpp"
#include "matrix.hpp"

int main() {
	size_t n_samples = 10;
	double ax = -1, bx = 1, ay = -1, by = 1;

	size_t n_interp = 100;

	std::vector<double> x(n_samples), y(n_samples);
	pp::Matrix<double> F(n_samples,n_samples);

	for (size_t i = 0; i < n_samples; ++i) {
		x[i] = ax + ((bx-ax) * i) / 9;
		y[i] = ay + ((by-ay) * i) / 9;
	}

	for (size_t j = 0; j < n_samples; ++j) 
		for (size_t i = 0; i < n_samples; ++i)
			F[i,j] = x[i] * y[j];

	for (size_t j = 0; j < n_samples; ++j) {
		for (size_t i = 0; i < n_samples; ++i) {
			std::cout << std::format("{} {} {}\n", x[i], y[j], F[i,j]);
		}
	}

	std::cout << "\n\n";
	
	double hx = (bx-ax)/n_interp, hy = (by-ay)/n_interp;

	double py = ay;
	for (size_t j = 0; j <= n_interp; ++j) {
		double px = ax;
		for (size_t i = 0; i <= n_interp; ++i) {
			std::cout << std::format("{} {} {}\n", px, py, pp::bilinear(x,y,F,px,py));
			px += hx;
		}
		py += hy;
	}
}
