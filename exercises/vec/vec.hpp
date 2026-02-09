#pragma once

#include <ostream>

namespace vec {
	struct vec {
		double x,y,z;
		double norm();

		vec(double x, double y, double z) : x(x), y(y), z(z) {}
		vec() : vec(0,0,0) {}
		vec(const vec&) = default;
		vec(vec&&) = default;
		~vec() = default;

		vec& operator =(const vec&) = default; //copy: a = b
		vec& operator =(vec&&) = default; //move: a = b + c

		vec& operator+=(const vec&);
		vec& operator-=(const vec&);
		// vec& operator+=(double);
		// vec& operator-=(double);
		vec& operator/=(double);
		vec& operator*=(double);

		friend std::ostream& operator<<(std::ostream&, const vec&);
	};

	vec operator -(vec);
	vec operator +(vec, const vec&);
	vec operator -(vec, const vec&);
	vec operator *(vec, double);
	vec operator *(double, vec);
	vec operator /(vec, double);
	double operator *(const vec&, const vec&); // dot product

	vec cross(const vec&, const vec&);
}
