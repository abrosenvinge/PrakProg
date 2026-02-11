#include "vec.hpp"
#include <cmath>
#include <ostream>
#include "../epsilon/approx.hpp"

namespace vec {
	vec& vec::operator +=(const vec& u) {
		this->x += u.x;
		this->y += u.y;
		this->z += u.z;
		return *this;
	}

	vec& vec::operator -=(const vec& u) {
		this->x -= u.x;
		this->y -= u.y;
		this->z -= u.z;
		return *this;
	}

	vec& vec::operator*=(double a) {
		this->x *= a;
		this->y *= a;
		this->z *= a;
		return *this;
	}

	vec& vec::operator/=(double a) {
		this->x /= a;
		this->y /= a;
		this->z /= a;
		return *this;
	}

	vec operator-(vec u) {
		u *= -1;
		return u;
	}

	// vec operator+(const vec& u, const vec& v) {
	// 	vec out = u;
	// 	out += v;
	// 	return out;
	// }
	
	// difference is whether u is passed by reference or copied when operator
	// is called. Apparently below version is better for cases like a+b+c
	vec operator+(vec u, const vec& v) {
		u += v;
		return u;
	}

	vec operator*(double a, vec u) {
		u *= a;
		return u;
	}

	vec operator*(vec u, double a) {
		u *= a;
		return u;
	}

	vec operator/(vec u, double a) {
		u /= a;
		return u;
	}

	double dot(const vec& u, const vec& v) {
		return u.x * v.x + u.y * v.y + u.z * v.z;
	}

	double vec::norm() {
		return std::sqrt(dot(*this, *this));
	}

	vec cross(const vec& u, const vec& v) {
		return vec(
			u.y*v.z - u.z*v.y,
			u.z*v.x - u.x*v.z,
			u.x*v.y - u.y*v.x
		);
	}

	bool approx(const vec& u, const vec& v) {
		return approx::approx(u.x,v.x)
				&& approx::approx(u.y,v.y)
				&& approx::approx(u.z,v.z);
	}

	std::ostream& operator<<(std::ostream& s, const vec& u) {
		s << u.x << " " << u.y << " " << u.z;
		return s;
	}
}
