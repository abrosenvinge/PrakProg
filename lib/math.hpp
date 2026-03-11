#include <algorithm>
#include <cstdlib>
namespace pp {
	template <typename T>
	bool approx(const T& a, const T& b, double acc = 1e9, double eps = 1e9) {
		double diff = std::abs(a - b);
		return diff <= acc || diff <= eps * std::max(std::abs(a), std::abs(b));
	}
}
