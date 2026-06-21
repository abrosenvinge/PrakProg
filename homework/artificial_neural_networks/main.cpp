#include "ann.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <vector>

pp::Vector<double> make_linear_start_parameters(size_t n_hidden_neurons, double a, double b) {
	pp::Vector<double> p(3*n_hidden_neurons);
	for (size_t i = 0; i < n_hidden_neurons; ++i) {
		p[3*i] = a + ((b-a)*i)/(n_hidden_neurons-1.);
		p[3*i+1] = 6. / n_hidden_neurons;
		p[3*i+2] = 8. / n_hidden_neurons;
	}
	return p;
}

pp::Vector<double> make_damped_oscillator_start_parameters(size_t n_hidden_neurons,
		double a, double b) {
	pp::Vector<double> p(3*n_hidden_neurons);
	for (size_t i = 0; i < n_hidden_neurons; ++i) {
		p[3*i] = a + ((b-a)*i)/(n_hidden_neurons-1.);
		p[3*i+1] = 5. / n_hidden_neurons;
		p[3*i+2] = 7. / n_hidden_neurons;
	}
	return p;
}

void make_plot_data(size_t n,
		pp::Vector<double>& p0,
		double a,
		double b,
		std::function<double(double)> f,
		std::function<double(double)> F,
		std::function<double(double)> df,
		std::function<double(double)> ddf,
		size_t n_training_points,
		size_t n_plot_points)
{
	pp::ann_Gaussian_wavelet ann(n, p0);

	std::vector<double> x(n_training_points), y(n_training_points);
	for (size_t i = 0; i < x.size(); ++i) {
		x[i] = a + (b-a)*i/(x.size()-1);
		y[i] = f(x[i]);
	}
	ann.train(x,y);

	for (size_t i = 0; i < x.size(); ++i) {
		double xi = x[i];
		std::cout << std::format("{} {} {} {} {}\n", xi, y[i], F(xi), df(xi), ddf(xi));
	}
	std::cout << "\n\n";
	double h = (b-a)/n_plot_points;
	for (double xi = a; xi <= b; xi += h) {
		std::cout << std::format("{} {} {} {} {}\n", xi, 
				ann.response(xi), 
				ann.integral(a,xi), 
				ann.deriv(xi),
				ann.second_deriv(xi));
	}

}

int main(int argc, char** argv) {
	size_t n = 5;
	double a = -1, b = 1;
	for (int i = 0; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "-n" && ++i < argc) n = std::stoi(argv[i]);
		else if (arg == "-a" && ++i < argc) a = std::stod(argv[i]);
		else if (arg == "-b" && ++i < argc) b = std::stod(argv[i]);
		else if (arg == "--linear") {
			pp::Vector<double> p0 = make_linear_start_parameters(n,a,b);
			auto f = [](double x) {return x;};
			auto F = [a](double x) {return 0.5*x*x - 0.5*a*a;};
			auto df = [](double) {return 1.;};
			auto ddf = [](double) {return 0.;};
			make_plot_data(n, p0, a, b, f, F, df, ddf, 50, 1000);
		}
		else if (arg == "--damped_oscillation") {
			pp::Vector<double> p0 = make_damped_oscillator_start_parameters(n,a,b);
			auto f = [](double x) {return std::cos(5*x-1)*std::exp(-x*x);};
			auto F = [](double) {return NAN;}; // I couldn't be bothered to calculate it
			auto df = [](double x) {
				return std::exp(-x*x) * (-5*std::sin(5*x-1)-2*x*std::cos(5*x-1));
			};
			auto ddf = [](double x) {
				return std::exp(-x*x)*((4*x*x-27)*std::cos(5*x-1)+20*x*std::sin(5*x-1));
			};
			make_plot_data(n, p0, a, b, f, F, df, ddf, 50, 1000);
		}
	}

	// std::cout << a.cost(x,y) << "\n\n";
	// std::cout << a.p << "\n\n";
	// std::cout << a.p << "\n\n";
	// std::cout << a.cost(x,y) << "\n\n";
	
	// pp::Vector<double> g(6);
	// pp::Matrix<double> H(6,6);
	// a.gradient_p(0, g);
	// a.hessian_p(0, H);
	// std::cout << g << "\n\n";
	// std::cout << H << "\n\n";
	// a.gradient_p(1, g);
	// a.hessian_p(1, H);
	// std::cout << g << "\n\n";
	// std::cout << H << "\n\n";
	//
	// a.cost_grad_hess_C(x, y, g, H);
	// std::cout << g << "\n\n";
	// std::cout << H << "\n\n";
}
