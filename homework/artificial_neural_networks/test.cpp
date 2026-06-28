#include "ann.hpp"
#include "../minimization/minimization.hpp"

int main() {
	size_t n = 2;
	pp::Vector<double> p0 {1,2,3,4,5,6};//,4,1,65,1,1,2};
	pp::ann_Gaussian_wavelet ann(n, p0);

	pp::Vector<double> grad2(3*n);
	pp::Matrix<double> H1(3*n,3*n), H2(3*n,3*n);

	std::vector<double> x{-2,-1,0,1,2,35.12354, 12.37923}, y{-2,-1,0,1,2,123,19.1823671};

	// ann.cost_grad_hess_C(x,y,grad1,H1);

	auto C = [&ann,&x, &y](const pp::Vector<double>& ptest) { return ann.cost(x,y,ptest); };
	// pp::gradient(C,p0, grad2);
	// pp::hessian(C, p0, H2);
	for (size_t i = 0; i < 100000; ++i) {
		pp::grad_hess_central(C,p0,grad2,H2);
	}

	// std::cout << grad1 << "\n\n" << H1 << "\n\n";

	std::cout << grad2 << "\n\n" << H2 << "\n\n";
	//
	// std::cout << H1 - H2 << "\n\n";
}
