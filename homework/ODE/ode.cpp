#include <functional>
#include <tuple>
#include <vector>
#include "ode.hpp"

namespace pp {
	std::tuple<std::vector<double>, std::vector<double>> rkstep45(
			const std::function<std::vector<double>(double, std::vector<double>&)> f,
			double x,
			const std::vector<double>& y,
			double step_size
	) {
		
	}

	std::tuple<std::vector<double>, std::vector<double>> driver(
			std::function<std::vector<double>(double, std::vector<double>&)> f,
			double a,
			double b,
			double initial_step_size,
			double acc,
			double eps
	);
}
