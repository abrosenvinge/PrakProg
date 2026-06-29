#include "bilinear.hpp"
#include "matrix.hpp"
#include "../homework/monte_carlo/monte_carlo.hpp"
#include <cmath>
#include <cstddef>
#include <functional>
#include <map>
#include <numbers>
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

void make_plot(const std::function<double(double,double)>& f,
		const std::vector<double>& x,
		const std::vector<double>& y,
		const pp::Matrix<double>& F,
		const std::vector<double>& xint,
		const std::vector<double>& yint,
		std::string mode,
		bool output)
{
		size_t n = xint.size();
		pp::Matrix<double> Fint, Iint(n,n);
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
			std::cout << "\n\n";
			print_nonuniform_matrix(xint, yint, evaluate_on_grid(f,xint,yint));
		}
}

void make_plot(
		const std::function<double(double,double)>& f,
		const std::function<double(double,double,double,double)>& I,
		const std::vector<double>& x,
		const std::vector<double>& y,
		const pp::Matrix<double>& F,
		const std::vector<double>& xint,
		const std::vector<double>& yint,
		std::string mode,
		bool output) 
{
		size_t n = xint.size();
		pp::Matrix<double> Iint(n,n), Iexact(n,n);

		for (size_t j = 0; j < n; ++j) {
			for (size_t i = 0; i < n; ++i) {
				Iint[i,j] = pp::integrate_bilinear(x,y,F,0,xint[i],0,yint[j]);
				Iexact[i,j] = I(0,xint[i],0,yint[j]);
			}
		}

		make_plot(f,x,y,F,xint,yint,mode,output);
		if (output) {
			std::cout << "\n\n";
			print_nonuniform_matrix(xint,yint,Iint);
			std::cout << "\n\n";
			print_nonuniform_matrix(xint,yint,Iexact);
		}
}

void test_optimized(std::string fname,
		const std::vector<double>& x,
		const std::vector<double>& y,
		const pp::Matrix<double>& F,
		const std::vector<double>& xint,
		const std::vector<double>& yint,
		bool output) 
{
	size_t n = xint.size();
	pp::Matrix<double> Fnorm(n,n);
	for (size_t j = 0; j < n; ++j) {
		for (size_t i = 0; i < n; ++i) {
			Fnorm[i,j] = pp::bilinear(x,y,F,xint[i],yint[j]);
		}
	}
	pp::Matrix<double> Fgrid = pp::bilinear(x,y,F,xint,yint);
	bool equal = pp::mat_approx(Fnorm, Fgrid);
	if (output)
		std::cout << std::format("Test for function {}: {}\n", fname, 
				equal ? "results are equal" : "results are not equal");
}

void test_integral(std::string fname,
		const std::function<double(double,double)>& f,
		const std::function<double(double,double,double,double)>& I,
		double ax, double bx, double ay, double by,
		const std::vector<double>& x,
		const std::vector<double>& y,
		const pp::Matrix<double>& F,
		bool output)
{
	size_t nx = x.size(), ny = y.size(), N = nx*ny;
	double correct_result = I(ax,bx,ay,by);

	double interpolation_result = pp::integrate_bilinear(x,y,F,ax,bx,ay,by);
	double interpolation_error = std::abs(correct_result - interpolation_result);

	auto [qrmc_res,_] = pp::quasirandmc([&f](const pp::Vector<double>& x){ return f(x[0],x[1]); }, {ax,ay},{bx,by},N);
	double qrmc_error = std::abs(correct_result - qrmc_res);

	if (output) {
		std::cout << std::format("Integral of \"{}\" from {} to {} and {} to {} with nx = {} and ny = {} (N = {})\n", fname, ax, bx, ay, by, nx, ny, N);
		std::cout << std::format("	{:<25} {:<25} {:<25}\n", "", "Result", "Error");
		std::cout << std::format("	{:<25} {:<25} {:<25}\n", "Exact", correct_result, "");
		std::cout << std::format("	{:<25} {:<25} {:<25}\n", "Bi-linear", interpolation_result, interpolation_error);
		std::cout << std::format("	{:<25} {:<25} {:<25}\n\n", "Quasi-random mc", qrmc_res, qrmc_error);
	}
}

int main(int argc, char** argv) {
	const std::map<std::string, std::function<double(double,double)>> functions {
		{"plane", [](double x, double y) { return -x+y; }},
		{"saddle", [](double x, double y) { return x*y; }},
		{"gaussian", [](double x, double y) { x += 3; y += 3; return std::exp(-x*x/8 - y*y/8); }},
		{"gauss_cos", [](double x, double y) { return std::exp(-x*x) * std::cos(5*y); }},
		{"wave", [](double x, double y) { return std::exp(-std::sqrt(x*x+y*y)/4)*std::cos(std::sqrt(x*x+y*y)); }},
	};
	
	const std::map<std::string, std::function<double(double,double,double,double)>> exact_integrals {
		{"plane", [](double ax, double bx, double ay, double by) { return -0.5*(ax-bx)*(ay-by)*(ax-ay+bx-by); }},
		{"saddle", [](double ax, double bx, double ay, double by) { return 0.25*(ax*ax-bx*bx)*(ay*ay-by*by); }},
		{"gauss_cos", [](double ax, double bx, double ay, double by) { 
							return 0.1*std::sqrt(std::numbers::pi)*(std::sin(5*ay)-std::sin(5*by))*(std::erf(ax)-std::erf(bx)); 
						}},
		{"gaussian", [](double ax, double bx, double ay, double by) { 
							ax += 3; bx += 3; ay += 3; by += 3;
							double c = 2*std::sqrt(2);
							return 2*std::numbers::pi*(std::erf(ax/c)-std::erf(bx/c))*(std::erf(ay/c)-std::erf(by/c)); 
						}},
	};

	std::string fname = "plane"; // name of the function
	double ax = -1, bx = 1, ay = -1, by = 1; // bounds for the grid
	size_t nx = 10, ny = 5; // number of grid points along each axis
	size_t n = 100; // number of grid points along each axis to interpolate
	std::string xspacing = "regular", yspacing = "regular", // how the sample grid points should be distributed along each axis ("regular" or "logarithmic")
		mode = "normal"; // whether to use the normal interpolation or the one intended for grid evaluation
	bool plot = false; // whether to make plot data
	bool plot_integral = false; // whether to plot the integrals
	bool output = true; // whether to output data for plotting
	bool test = false; // whether to test the two methods against eachother
	bool integral = false; // whether to test the integrals
	
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
		else if (arg == "-F" && ++i < argc) fname = argv[i];
		else if (arg == "--xspacing" && ++i < argc) xspacing = argv[i];
		else if (arg == "--yspacing" && ++i < argc) yspacing = argv[i];
		else if (arg == "--plot") plot = true; 
		else if (arg == "--plot_integral") plot_integral = true; 
		else if (arg == "--output") output = true; 
		else if (arg == "--nooutput") output = false; 
		else if (arg == "--test") test = true; 
		else if (arg == "--mode" && ++i < argc) mode = argv[i];
		else if (arg == "--integrate") integral = true;

	}

	const std::function<double(double,double)>& f = functions.at(fname); // function to interpolate

	std::vector<double> x,y;
	if (xspacing == "regular") x = make_regular_grid_axis(ax,bx,nx);
	else if (xspacing == "logarithmic") x = make_logarithmic_grid_axis(ax,bx,nx); 

	if (yspacing == "regular") y = make_regular_grid_axis(ay,by,ny);
	else if (yspacing == "logarithmic") y = make_logarithmic_grid_axis(ay,by,ny); 

	pp::Matrix<double> F = evaluate_on_grid(f,x,y);

	std::vector<double> xint = make_regular_grid_axis(ax,bx,n), 
						yint = make_regular_grid_axis(ay,by,n);

	if (plot) {
		if (plot_integral) make_plot(f,exact_integrals.at(fname), x,y,F,xint,yint,mode,output);
		else make_plot(f,x,y,F,xint,yint,mode,output);
	}

	if (test) test_optimized(fname,x,y,F,xint,yint,output);

	if (integral) {
		test_integral(fname,f,exact_integrals.at(fname),ax,bx,ay,by,x,y,F,output);
	}
}
