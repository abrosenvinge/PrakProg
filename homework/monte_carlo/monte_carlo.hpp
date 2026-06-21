#pragma once
#include <cmath>
#include <random>
#include <tuple>
#include <vector>
#include <functional>
#include "../lib/matrix.hpp"

namespace pp {
	template <class RAND_TYPE>
	std::tuple<double, double> plainmc(const std::function<double(const pp::Vector<double>&)>& f,
										const pp::Vector<double>& a,
										const pp::Vector<double>& b,
										size_t N,
										RAND_TYPE& rand_gen) 
	{
		size_t dim = a.size;
		double V = 1;
		for (size_t i = 0; i < dim; ++i) V *= b[i] - a[i];
		double sum = 0., sq_sum = 0.;
		pp::Vector<double> x(dim);
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < dim; ++j) x[j] = a[j] + rand_gen() * (b[j] - a[j]);
			double fx = f(x);
			sum += fx;
			sq_sum += fx*fx;
		}
		double mean = sum/N;
		double standard_deviation = std::sqrt(sq_sum / N - mean*mean);

		return std::tuple(mean * V, standard_deviation * V / std::sqrt(N));
	}

	class LCG {
	public:
		long seed; // long is a (poor) solution so it doesn't overflow.
		long a, c, m;
		LCG(long seed, long a = 1664525, long c = 1013904223, long m = std::pow(2,32)) : seed(seed), a(a), c(c), m(m) {};
		
		double operator()() {
			seed = (a * seed + c) % m;
			return ((double) (seed+1))/(m+1);
		}
	}; // class LCG

	// thin wrapper around std::uniform_real_distribution so calls don't require an argument
	class std_uniform_dist {
	public:
		std::uniform_real_distribution<double> random;
		std::default_random_engine re;

		std_uniform_dist() : random(0.,1.) {}

		double operator()() {
			return random(re);
		}
	};

	std::vector<int> generate_primes(size_t n);

	double corput(size_t n, int b);

	class Halton {
	public:
		std::vector<int> primes;
		Halton(size_t n) : primes(generate_primes(n)) {}

		pp::Vector<double> operator()(size_t n);
	};

	std::tuple<double, double> quasirandmc(const std::function<double(const pp::Vector<double>&)>& f,
										const pp::Vector<double>& a,
										const pp::Vector<double>& b,
										size_t N);

	class Lattice {
	public:
		std::vector<double> alphas;
		Lattice(size_t n) {
			for (int p : generate_primes(n)) alphas.push_back(std::fmod(std::sqrt(p), 1));
		}

		pp::Vector<double> operator()(size_t n);
	};

	std::tuple<double, double> stratified(const std::function<double(const pp::Vector<double>&)>& f,
										const pp::Vector<double>& a,
										const pp::Vector<double>& b,
										size_t N,
										size_t nmin = 1000);

	std::tuple<double, double> stratified_recursive(const std::function<double(const pp::Vector<double>&)>& f,
										const pp::Vector<double>& a,
										const pp::Vector<double>& b,
										size_t N,
										size_t nmin,
										std_uniform_dist rand_gen);


} //namespace pp
