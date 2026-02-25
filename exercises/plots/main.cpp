#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <numbers>
#include <limits>

constexpr double PI = std::numbers::pi;
constexpr double NaN = std::numeric_limits<double>::quiet_NaN();

double lngamma(double x){
	if(x<=0) return NaN;
	if(x<9) return lngamma(x+1)-std::log(x);
	return x*std::log(x+1/(12*x-1/x/10))-x+std::log(2*PI/x)/2;
}

double sgamma(double x){
	if(x<0)return PI/std::sin(PI*x)/sgamma(1-x);
	if(x<9)return sgamma(x+1)/x;
	double lnsgamma=std::log(2*PI)/2+(x-0.5)*std::log(x)-x
		+(1.0/12)/x-(1.0/360)/(x*x*x)+(1.0/1260)/(x*x*x*x*x);
	return std::exp(lnsgamma);
}

double erf(double x){
	// single precision error function (Abramowitz and Stegun, from Wikipedia)
	if(x<0) return -erf(-x);
	std::vector<double> a {0.254829592,-0.284496736,1.421413741,-1.453152027,1.061405429};
	double t=1/(1+0.3275911*x);
	double sum=t*(a[0]+t*(a[1]+t*(a[2]+t*(a[3]+t*a[4]))));/* the right thing */
	return 1-sum*std::exp(-x*x);
} 

int main(int argc, char** argv) {
	std::function<double(double)> f = erf;
	double xmin = 0;
	double xmax = 10;
	double dx = 0.01;

	for (int i = 0; i < argc; i++) {
		std::string arg = argv[i];

		if (arg == "--erf") f = erf;
		else if (arg == "--gamma") f = sgamma;
		else if (arg == "--lngamma") f = lngamma;
		else if (arg == "--xmin" && ++i < argc) xmin = std::stod(argv[i]);
		else if (arg == "--xmax" && ++i < argc) xmax = std::stod(argv[i]);
		else if (arg == "--dx" && ++i < argc) dx = std::stod(argv[i]);
	}

	if (xmin < xmax) {
		for (double x = xmin; x < xmax; x += dx) {
			std::cout << x << " " << f(x) << "\n";
		}
	}
}
