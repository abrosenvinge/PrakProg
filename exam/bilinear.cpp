#include "bilinear.hpp"
#include <algorithm>
#include <utility>
#include <vector>
#include "matrix.hpp"
#include "splines.hpp"

namespace pp {
	// evaluate the bilinear interpolation of F on the grid x,y at the point (px,py) in the 
	// case where the rectangle (i,j) containing (px,py) is already known
	double bilinear(const std::vector<double>& x, 
			const std::vector<double>& y,
			const Matrix<double>& F,
			double px,
			double py,
			size_t i,
			size_t j)
	{
		double xi = x[i], yj = y[j];
		double dx = x[i+1] - xi;
		double dy = y[j+1] - yj;

		double a = F[i,j];
		double b = (F[i+1,j] - a)/dx;
		double c = (F[i,j+1] - a)/dy;
		double d = (F[i+1,j+1] - a - b*dx - c*dy) / (dx*dy);

		return a + b*(px-xi) + c*(py-yj) + d*(px-xi)*(py-yj);
	}

	double bilinear(const std::vector<double>& x, 
			const std::vector<double>& y,
			const Matrix<double>& F,
			double px,
			double py)
	{
		if (px < x[0] || px > x.back()) throw std::out_of_range(std::format("px = {} not between first and last element of x ({}, {})", px, x[0], x.back()));
		if (py < y[0] || py > y.back()) throw std::out_of_range(std::format("py = {} not between first and last element of y ({}, {})", py, y[0], y.back()));
		size_t i = binary_search(x, px);
		size_t j = binary_search(y, py);
		
		return bilinear(x,y,F,px,py,i,j);
	}

	Matrix<double> bilinear(const std::vector<double>& x, 
			const std::vector<double>& y,
			const Matrix<double>& F,
			const std::vector<double>& px,
			const std::vector<double>& py) 
	{
		if (px[0] < x[0] || px.back() > x.back())
			throw std::out_of_range(std::format("px interval ({},{}) not between first and last element of x ({}, {})", px[0],px.back(), x[0], x.back()));
		if (py[0] < y[0] || py.back() > y.back())
			throw std::out_of_range(std::format("py interval ({},{}) not between first and last element of y ({}, {})", py[0],py.back(), y[0], y.back()));
		Matrix<double> out(px.size(), py.size());
		size_t j = 0;
		for (size_t pj = 0; pj < py.size(); ++pj) {
			double pyj = py[pj];
			while (pyj > y[j+1]) j++;
			size_t i = 0;
			for (size_t pi = 0; pi < px.size(); ++pi) {
				double pxi = px[pi];
				while (pxi > x[i+1]) i++;

				out[pi,pj] = bilinear(x,y,F,pxi,pyj,i,j);
			}
		}
		return out;
	}

	double integrate_bilinear_rectangle(const std::vector<double>& x, 
			const std::vector<double>& y,
			const Matrix<double>& F,
			double ax, double bx,
			double ay, double by,
			size_t i, size_t j)
	{
		double xi = x[i];
		double yj = y[j];
		
		double dx = x[i+1] - xi;
		double dy = y[j+1] - yj;

		double a = F[i,j];
		double b = (F[i+1,j] - a)/dx;
		double c = (F[i,j+1] - a)/dy;
		double d = (F[i+1,j+1] - a - b*dx - c*dy) / (dx*dy);

		double dxa = ax - xi;
		double dxb = bx - xi;
		double dya = ay - yj;
		double dyb = by - yj;

		double hx = bx - ax;
		double hy = by - ay;

		double xterm = dxb*dxb-dxa*dxa;
		double yterm = dyb*dyb-dya*dya;

		return a*hx*hy + 0.5*b*xterm*hy + 0.5*c*yterm*hx + 0.25*d*xterm*yterm;
	}

	double integrate_bilinear(const std::vector<double>& x, 
			const std::vector<double>& y,
			const Matrix<double>& F,
			double ax, double bx,
			double ay, double by) 
	{
		double f = 1;
		if (bx < ax) {
			std::swap(ax,bx);
			f *= -1;
		}
		if (by < ay) {
			std::swap(ay,by);
			f *= -1;
		}
		if (ax < x[0] || bx > x.back())
			throw std::out_of_range(std::format("integration x interval ({},{}) not between first and last element of x ({}, {})", ax , bx, x[0], x.back()));
		if (ay < y[0] || by > y.back())
			throw std::out_of_range(std::format("integration y interval ({},{}) not between first and last element of y ({}, {})", ay , by, y[0], y.back()));

		size_t i = binary_search(x, ax);
		size_t jstart = binary_search(y, ay);

		double sum = 0;

		double lx = ax;
		double ux;

		while (lx < bx) {
			ux = std::min(bx, x[i+1]);

			size_t j = jstart;
			double ly = ay, uy;
			while (ly < by) {
				uy = std::min(by, y[j+1]);
				sum += integrate_bilinear_rectangle(x,y,F,lx,ux,ly,uy,i,j);
				
				ly = uy;
				j++;
			}

			lx = ux;
			i++;
		}

		return f*sum;
	}

}
