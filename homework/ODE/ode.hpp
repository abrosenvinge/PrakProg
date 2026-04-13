#include <array>
#include <functional>
#include <tuple>
#include <vector>

namespace pp {

	class StepperRK45 {
	private:
		using FUNC = const std::function<void(double, const std::vector<double>&, std::vector<double>&)>&;
		std::array<double, 5> k;
		std::vector<double> y;
		FUNC f;
	public:
		StepperRK45(FUNC f, std::vector<double> y0) : f(f), y(y0) {}

		
	};

	std::tuple<std::vector<double>, std::vector<double>> rkstep45(
			const std::function<std::vector<double>(double, std::vector<double>&)>,
			double,
			const std::vector<double>&,
			double
	);

	std::tuple<std::vector<double>, std::vector<double>> driver(
			std::function<std::vector<double>(double, std::vector<double>&)>,
			double,
			double,
			double,
			double,
			double
	);
}
