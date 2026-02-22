#pragma once

#include <ostream>
#include <cmath>
#include "../epsilon/approx.hpp"

namespace vec {
	template <typename T>
	struct vec {
		T x,y,z;
		double norm();

		// constructors
		vec(T x, T y, T z) {
			this->x = x;
			this->y = y;
			this->z = z;
		} // parametrized
		vec() : vec(0,0,0) {} // default
		vec(const vec&) = default; // copy: a(b)
		vec(vec&&) = default; // move: a(b+c)

		// destructor
		~vec() = default;

		// assignment
		vec& operator =(const vec&) = default; //copy: a = b
		vec& operator =(vec&&) = default; //move: a = b + c

		vec& operator+=(const vec& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		vec& operator-=(const vec& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		vec& operator/=(T a) {
			x /= a;
			y /= a;
			z /= a;
			return *this;
		}
		vec& operator*=(T a) {
			x *= a;
			y *= a;
			z *= a;
			return *this;
		}

		friend std::ostream& operator<<(std::ostream& s, const vec& u) {
			s << u.x << " " << u.y << " " << u.z;
			return s;
		}
	};

	template<typename T>
	vec<T> operator -(vec<T> u) {
		u *= -1;
		return u;
	}

	template<typename T>
	vec<T> operator+(vec<T> u, const vec<T>& v) {
		u += v;
		return u;
	}

	template<typename T>
	vec<T> operator*(T a, vec<T> u) {
		u *= a;
		return u;
	}

	template<typename T>
	vec<T> operator*(vec<T> u, T a) {
		u *= a;
		return u;
	}

	template<typename T>
	vec<T> operator/(vec<T> u, T a) {
		u /= a;
		return u;
	}

	template<typename T>
	double dot(const vec<T>& u, const vec<T>& v) {
		return u.x * v.x + u.y * v.y + u.z * v.z;
	}

	template<typename T>
	double vec<T>::norm() {
		return std::sqrt(dot(*this, *this));
	}

	template<typename T>
	vec<T> cross(const vec<T>& u, const vec<T>& v) {
		return vec(
			u.y*v.z - u.z*v.y,
			u.z*v.x - u.x*v.z,
			u.x*v.y - u.y*v.x
		);
	}

	template<typename T>
	bool approx(const vec<T>& u, const vec<T>& v) {
		return approx::approx(u.x,v.x)
				&& approx::approx(u.y,v.y)
				&& approx::approx(u.z,v.z);
	}
}
