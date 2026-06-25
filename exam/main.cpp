#include "bilinear.hpp"
#include "matrix.hpp"
#include <cstddef>
#include <functional>
#include <string>
#include <vector>

std::vector<double> make_regular_grid_axis(double a, double b, size_t n) {
	std::vector<double> x(n);
	for (size_t i = 0; i < n; ++i) {
		x[i] = a + ((b-a) * i) / (n - 1);
	}
	return x;
}

std::vector<double> make_logarithmic_grid_axis(double a, double b, size_t n) {
	std::vector<double> x(n);

	double c = std::pow(b-a+1,1./(n-1));

	for (size_t i = 0; i < n-1; ++i) {
		x[i] = a + std::pow(c,i) - 1;
	}
	x[x.size()-1] = b; // otherwise the grid may not contain b due to rounding errors
	return x;
}

pp::Matrix<double> evaluate_on_grid(const std::function<double(double,double)>& f, 
		const std::vector<double>& x,
		const std::vector<double>& y) 
{
	pp::Matrix<double> out(x.size(),y.size());
	for (size_t j = 0; j < out.n_cols; ++j)
		for (size_t i = 0; i < out.n_rows; ++i)
			out[i,j] = f(x[i],y[j]);
	return out;
}

void print_nonuniform_matrix(
		const std::vector<double>& x,
		const std::vector<double>& y,
		const pp::Matrix<double>& F)
{
	size_t ny = y.size(), nx = x.size();
	std::cout << ny << " ";
	for (size_t i = 0; i < nx; ++i) std::cout << x[i] << " ";
	std::cout << "\n";

	for (size_t j = 0; j < ny; ++j) {
		std::cout << y[j] << " ";
		for (size_t i = 0; i < nx; ++i) {
			std::cout << F[i,j] << " ";
		}
		std::cout << "\n";
	}
}

int main(int argc, char** argv) {
	double ax = -1, bx = 1, ay = -1, by = 1; // bounds for the grid
	size_t nx = 10, ny = 5; // number of grid points along each axis
	size_t n = 100; // number of grid points along each axis to interpolate
	std::function<double(double,double)> f = [](double x, double y) { return x*y; }; // function to interpolate
	std::string xspacing = "regular", yspacing = "regular", // how the sample grid points should be distributed along each axis ("regular" or "logarithmic")
		mode = "normal"; // whether to use the normal interpolation or the one intended for grid evaluation
	bool output = true; // whether to output data for plotting
	
	for (int i = 0; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "-nx" && ++i < argc) nx = std::stoi(argv[i]);
		else if (arg == "-ny" && ++i < argc) ny = std::stoi(argv[i]);
		else if (arg == "-n" && ++i < argc) n = std::stoi(argv[i]);
		else if (arg == "--xbounds" && i+2 < argc) {
			ax = std::stod(argv[i+1]);
			bx = std::stod(argv[i+2]);
			i += 2;
		}
		else if (arg == "--ybounds" && i+2 < argc) {
			ay = std::stod(argv[i+1]);
			by = std::stod(argv[i+2]);
			i += 2;
		}
		else if (arg == "-F" && ++i < argc) {
			std::string fname = argv[i];
			if (fname == "plane") f = [](double x, double y) { return -x+y; };
			else if (fname == "saddle") f = [](double x, double y) { return x*y; };
			else if (fname == "gaussian") 
				f = [](double x, double y) { x += 3; y += 3; return std::exp(-x*x/8 - y*y/8); };
			else if (fname == "gauss_cos") 
				f = [](double x, double y) { return std::exp(-x*x) * std::cos(5*y); };
		}
		else if (arg == "--xspacing" && ++i < argc) xspacing = argv[i];
		else if (arg == "--yspacing" && ++i < argc) yspacing = argv[i];
		else if (arg == "--output") output = true; 
		else if (arg == "--nooutput") output = false; 
		else if (arg == "--mode" && ++i < argc) mode = argv[i];
	}

	std::vector<double> x,y;
	if (xspacing == "regular") x = make_regular_grid_axis(ax,bx,nx);
	else if (xspacing == "logarithmic") x = make_logarithmic_grid_axis(ax,bx,nx); 

	if (yspacing == "regular") y = make_regular_grid_axis(ay,by,ny);
	else if (yspacing == "logarithmic") y = make_logarithmic_grid_axis(ay,by,ny); 

	pp::Matrix<double> F = evaluate_on_grid(f,x,y);

	std::vector<double> xint = make_regular_grid_axis(ax,bx,n), 
						yint = make_regular_grid_axis(ay,by,n);

	pp::Matrix<double> Fint;
	if (mode == "normal") {
		Fint = pp::Matrix<double>(n,n);
		for (size_t j = 0; j < n; ++j) {
			for (size_t i = 0; i < n; ++i) {
				Fint[i,j] = pp::bilinear(x,y,F,xint[i],yint[j]);
			}
		}
	}
	else if (mode == "grid") Fint = pp::bilinear(x,y,F,xint,yint);


	if (output) {
		print_nonuniform_matrix(x,y,F);
		std::cout << "\n\n";
		print_nonuniform_matrix(xint, yint, Fint);
	}
}
