#include "monte_carlo.hpp"
#include <cmath>
#include <vector>
#include "../lib/matrix.hpp"

namespace pp {
	std::vector<int> generate_primes(size_t n) {
		std::vector<int> out;
		int candidate = 2;

		while (out.size() < n) {
			bool is_prime = true;
			for (int p : out) {
				if (p*p > candidate) break;
				if (candidate % p == 0) {
					is_prime = false;
					break;
				}
			}
			if (is_prime) out.push_back(candidate);
			candidate++;
		}
		return out;
	}

	double corput(size_t n, int b) {
		double out = 0., bk = 1./b;
		while (n > 0) {
			out += (n % b) * bk;
			n /= b;
			bk /= b;
		}
		return out;
	}

	pp::Vector<double> Halton::operator()(size_t n) {
		size_t N = primes.size();
		pp::Vector<double> out(N);
		for (size_t i = 0; i < N; ++i) {
			out[i] = corput(n, primes[i]);
		}
		return out;
	}

	pp::Vector<double> Lattice::operator()(size_t n) {
		size_t N = alphas.size();
		pp::Vector<double> out(N);
		for (size_t i = 0; i < N; ++i) {
			out[i] = std::fmod(n*alphas[i], 1);
		}
		return out;
	}

	std::tuple<double, double> quasirandmc(const std::function<double(const pp::Vector<double>&)>& f,
										const pp::Vector<double>& a,
										const pp::Vector<double>& b,
										size_t N) {
		size_t dim = a.size;
		Halton rand1(dim);
		Lattice rand2(dim);

		double V = 1;
		for (size_t i = 0; i < dim; ++i) V *= b[i] - a[i];
		double sum1 = 0., sum2 = 0.;

		pp::Vector<double> x1(dim), x2(dim);
		for (size_t i = 0; i < N; ++i) {
			x1 = rand1(i+1); x2 = rand2(i+1);
			for (size_t j = 0; j < dim; ++j) {
				x1[j] *= (b[j] - a[j]); x1[j] += a[j];
				x2[j] *= (b[j] - a[j]); x2[j] += a[j];
			}
			sum1 += f(x1);
			sum2 += f(x2);
		}
		double mean1 = sum1/N;
		double mean2 = sum2/N;

		return std::tuple(0.5 * (mean1 + mean2) * V, std::abs(mean1 - mean2) * V);
	}
	std::tuple<double, double> stratified(const std::function<double(const pp::Vector<double>&)>& f,
										const pp::Vector<double>& a,
										const pp::Vector<double>& b,
										size_t N,
										size_t nmin) 
	{
		std_uniform_dist rand_gen;
		return stratified_recursive(f,a,b,N,nmin,rand_gen);
	}

	std::tuple<double, double> stratified_recursive(const std::function<double(const pp::Vector<double>&)>& f,
										const pp::Vector<double>& a,
										const pp::Vector<double>& b,
										size_t N,
										size_t nmin,
										std_uniform_dist rand_gen)
	{
		if (N == 0) return std::tuple(0,0);
		else if (N <= nmin) return plainmc(f,a,b,N,rand_gen);
		else {
			size_t dim = a.size;
			double V = 1.;
			for (size_t i = 0; i < dim; ++i) V *= b[i] - a[i];
			
			std::vector<int> n_left(dim), n_right(dim);
			std::vector<double> sum_left(dim), sum_right(dim), sum_sq_left(dim), sum_sq_right(dim);
			pp::Vector<double> x(dim);
			for (size_t i = 0; i < nmin; ++i) {
				for (size_t j = 0; j < dim; ++j) x[j] = a[j] + rand_gen() * (b[j] - a[j]);
				double fx = f(x);
				
				for (size_t j = 0; j < dim; ++j) {
					if (x[j] < 0.5 * (b[j] + a[j])) {
						n_left[j]++;
						sum_left[j] += fx;
						sum_sq_left[j] += fx*fx;
					} 
					else {
						n_right[j]++;
						sum_right[j] += fx;
						sum_sq_right[j] += fx*fx;
					}
				}
			}
			size_t max_var_dim = 0;
			double max_var = 0.;
			double var_left_max, var_right_max;
			for (size_t i = 0; i < dim; ++i) {
				// double var = std::abs(sum_right[i]/n_right[i] - sum_left[i]/n_left[i]);
				double var_left = sum_sq_left[i]/n_left[i] - std::pow(sum_left[i]/n_left[i], 2);
				double var_right = sum_sq_left[i]/n_left[i] - std::pow(sum_left[i]/n_left[i], 2);
				double var = var_left + var_right;
				if (var > max_var) {
					max_var_dim = i;
					max_var = var;
					var_left_max = var_left;
					var_right_max = var_right;
				}
			}
			size_t n_left_max = n_left[max_var_dim];
			size_t n_right_max = n_right[max_var_dim];
			double sum_left_max = sum_left[max_var_dim];
			double sum_right_max = sum_right[max_var_dim];

			double total_var_max = var_left_max + var_right_max;

			size_t N_remaining = N - nmin;

			size_t N_left,N_right;
			if (var_left_max <= 0. && var_right_max <= 0.) {
				N_left = N_remaining / 2; N_right = N_remaining - N_left;
			}
			else {
				N_left = (size_t) (N_remaining * var_left_max/total_var_max);
				N_right = N_remaining - N_left;
			}

			size_t n_left_tot = N_left + n_left_max;
			size_t n_right_tot = N_right + n_right_max;

			pp::Vector<double> b_left = b, a_right = a;
			b_left[max_var_dim] = 0.5 * (a[max_var_dim] + b[max_var_dim]);
			a_right[max_var_dim] = 0.5 * (a[max_var_dim] + b[max_var_dim]);

			auto [left,left_err] = stratified_recursive(f, a, b_left, N_left, nmin, rand_gen);
			auto [right,right_err] = stratified_recursive(f, a_right, b, N_right, nmin, rand_gen);

			double total_left = (0.5*V * sum_left_max + left * N_left) / n_left_tot;
			double total_right = (0.5*V * sum_right_max + right * N_right) / n_right_tot;
			double total_var_left = (0.25*V*V*var_left_max + left_err*left_err*N_left*N_left)/(n_left_tot*n_left_tot);
			double total_var_right = (0.25*V*V*var_right_max + right_err*right_err*N_right*N_right)/(n_right_tot*n_right_tot);

			return std::tuple(total_left + total_right, std::sqrt(total_var_left + total_var_right));
		}
	}
}
