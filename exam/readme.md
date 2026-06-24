# Exam problem
**Bi-linear interpolation on a rectilinear grid in two dimensions**

## Introduction
A rectilinear grid (note that rectilinear is not necessarily cartesian nor regular) in two dimensions is a set of $n_x\times n_y$ points where each point can be adressed by a double index $(i,j)$ where $1 \leq i \leq n_x$, $1 \leq j \leq n_y$ and the coordinates of the point $(i,j)$ are given as $(x_i,y_j)$, where $x$ and $y$ are vectors with sizes $n_x$ and $n_y$ correspondingly. The values of the tabulated function $F$ at the grid points can then be arranged as a matrix ${F_i, j=F(x_i,y_j)}$. 
## Problem
Build an interpolating routine which takes as the input the vectors {xi} and {yj}, and the matrix {Fi,j} and returns the bi-linear interpolated value of the function at a given 2D-point p=(px,py). 
## Hints
See the chapter "Bi-linear interpolation" in the book.

The signature of the interpolating subroutine can be

static double bilinear(double[] x, double[] y, matrix F, double px, double py)

# Solution


