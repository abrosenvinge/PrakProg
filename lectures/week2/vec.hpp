#pragma once
#include <iostream>
#include <string>
namespace pp {
	struct vec3 {
		double x,y,z;

		// ctors
		vec3(double a, double b, double c) { // parametrized constructor (parm, ctor)
			std::cout << "Parm ctor called\n";
			x = a; y = b; z = c;
		}

		vec3() : vec3(0,0,0) {
			std::cout << "default constructor called\n";
		}
		vec3(const vec3&) = default; //copy ctor: vec a = b
		vec3(vec3&&) = default; //move ctor: vec a = b + c

		//dtor
		~vec3(){
			std::cout << "destructor called\n";
		}

		//assignments
		vec3& operator =(const vec3&); //copy: a = b
		vec3& operator =(vec3&&); //move: a = b + c

		//operations
		vec3& operator +=(double);
		vec3& operator -=(double);
		vec3& operator *=(double);
		vec3& operator /=(double);

		void print(const std::string s="");

		// stream output
		friend std::ostream& operator<<(std::ostream&, const vec3&);
	};

	vec3 operator -(const vec3&);
	vec3 operator +(const vec3&, const vec3&);
	vec3 operator -(const vec3&, const vec3&);
	vec3 operator *(const vec3&, double);
	vec3 operator *(double, const vec3&);
	vec3 operator /(const vec3&, double);
}
