# Exam problem
**Bi-linear interpolation on a rectilinear grid in two dimensions**

## Introduction
A rectilinear grid (note that rectilinear is not necessarily cartesian nor regular) in two dimensions is a set of $n_x\times n_y$ points where each point can be adressed by a double index $(i,j)$ where $1 \leq i \leq n_x$, $1 \leq j \leq n_y$ and the coordinates of the point $(i,j)$ are given as $(x_i,y_j)$, where $x$ and $y$ are vectors with sizes $n_x$ and $n_y$ correspondingly. The values of the tabulated function $F$ at the grid points can then be arranged as a matrix $\{F_i, j=F(x_i,y_j)\}$. 
## Problem
Build an interpolating routine which takes as the input the vectors $\{x_i\}$ and $\{y_j\}$, and the matrix $\{F_{i,j}\}$ and returns the bi-linear interpolated value of the function at a given 2D-point $p=(p_x,p_y)$. 
## Hints
See the chapter "Bi-linear interpolation" in the book.

The signature of the interpolating subroutine can be

```c++
static double bilinear(double[] x, double[] y, matrix F, double px, double py)
```

# Solution

## Mathematical solution
To interpolate $F$ inside the rectangle with corners $(x_i,y_j)$, $(x_{i+1},y_j)$, $(x_i,y_{j+1})$, $(x_{i+1},y_{i+1})$ we must, according to the book, determine the coefficient in the bilinear function 
```math
B_{i,j}(x,y) = a_{i,j} + b_{i,j}x + c_{i,j}y + d_{i,j}xy,
```
such that it takes the correct values at the corners. 
```math
B_{i,j}(x_i,y_j) = F_{i,j},
```
```math
B_{i,j}(x_{i+1},y_j) = F_{i+1,j},
```
```math
B_{i,j}(x_{i},y_{j+1}) = F_{i,j+1},
```
and
```math
B_{i,j}(x_{i+1},y_{j+1}) = F_{i+1,j+1}.
```

It turns out however, that this is simpler to solve by redefining the coefficients such that 
```math
B_{i,j}(x,y) = a_{i,j} + b_{i,j}(x-x_i) + c_{i,j}(y-y_i) + d_{i,j}(x-x_i)(y-y_i).
```
In this case the above equations can be easily solved to give 
```math
a = F_{i,j},
```
```math
b = \frac{F_{i+1,j} - a}{\Delta x},
```
```math
c = \frac{F_{i,j+1} - a}{\Delta y},
```
and
```math
d = \frac{F_{i+1,j+1} - a - b\Delta x - c\Delta y}{\Delta x\Delta y}.
```
