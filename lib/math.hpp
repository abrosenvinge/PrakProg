#include <algorithm>
#include <cstdlib>
namespace pp {
	template <typename T>
	bool approx(const T& a, const T& b, double acc, double eps) {
		double diff = std::abs(a - b);
		return diff <= acc || diff <= eps * std::max(std::abs(a), std::abs(b));
	}
}
