# Exam problem
**Bi-linear interpolation on a rectilinear grid in two dimensions**

## Introduction
A rectilinear grid (note that rectilinear is not necessarily cartesian nor regular) in two dimensions is a set of $n_x\times n_y$ points where each point can be adressed by a double index $(i,j)$ where $1 \leq i \leq n_x$, $1 \leq j \leq n_y$ and the coordinates of the point $(i,j)$ are given as $(x_i,y_j)$, where $x$ and $y$ are vectors with sizes $n_x$ and $n_y$ correspondingly. The values of the tabulated function $F$ at the grid points can then be arranged as a matrix $\{F_i, j=F(x_i,y_j)\}$. 
## Problem
### a
Build an interpolating routine which takes as the input the vectors $\{x_i\}$ and $\{y_j\}$, and the matrix $\{F_{i,j}\}$ and returns the bi-linear interpolated value of the function at a given 2D-point $p=(p_x,p_y)$. 
#### Hints
See the chapter "Bi-linear interpolation" in the book.

The signature of the interpolating subroutine can be
```c++
static double bilinear(double[] x, double[] y, matrix F, double px, double py)
```

### b
The bilinear functions can easily be integrated exactly. Use this to approximate the integral of a function
```math
\int_{a_y}^{b_y} \int_{a_x}^{b_x} f(x,y) dx dy
```

### c
It is sometimes useful to evaluate the interpolation at every point on a denser grid given by two vectors $x'$ and $y'$ (for instance for plotting). Because the above function takes only a single point at a time, it is necessary to perform two binary searches for each point. Evaluation on an $N\times N$ grid is therefore $O(N^2 log(N))$.

These binary searches are unnecessary however, because the grid points $x'$ and $y'$ are sorted. Therefore, it is possible to evaluate the interpolation on a grid in $O(N^2)$.

Implement this and plot the time taken for the two methods as a function of $N$.

# Solution
### a
#### Mathematical solution
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
a_{i,j} = F_{i,j},
```
```math
b_{i,j} = \frac{F_{i+1,j} - a}{\Delta x},
```
```math
c_{i,j} = \frac{F_{i,j+1} - a}{\Delta y},
```
and
```math
d_{i,j} = \frac{F_{i+1,j+1} - a - b\Delta x - c\Delta y}{\Delta x\Delta y},
```
where $\Delta x = x_{i+1}-x_i$ and $\Delta y = y_{j+1} - y_j$.

#### Implementation
The algorithm itself is implemented in [bilinear.hpp](./bilinear.hpp) and [bilinear.cpp](./bilinear.cpp) with the signature
```c++
double bilinear(const std::vector<double>& x, const std::vector<double>& y, const Matrix<double>& F, double px, double py);
```
where Matrix is implemented in [matrix.hpp](../homework/lib/matrix.hpp) which is taken from the homework via symlink.

The implementation finds the rectangle containing $(p_x, p_y)$ using two binary searches implemented in [splines.hpp](../homework/splines/splines.hpp) (symlink to the splines homework): 
```c++
size_t i = binary_search(x, px);
size_t j = binary_search(y, py);
```

Following this the coefficients are computed as described above and $B_{i,j}(p_x,p_y)$ is returned, which is done in the function 
```c++
double bilinear(const std::vector<double>& x, 
        const std::vector<double>& y,
        const Matrix<double>& F,
        double px,
        double py,
        size_t i,
        size_t j) 
{
    double dx = x[i+1] - x[i];
    double dy = y[j+1] - y[j];

    double a = F[i,j];
    double b = (F[i+1,j] - a)/dx;
    double c = (F[i,j+1] - a)/dy;
    double d = (F[i+1,j+1] - a - b*dx - c*dy) / (dx*dy);

    return a + b*(px-x[i]) + c*(py-y[j]) + d*(px-x[i])*(py-y[j]);
}
```

#### Plots
To test the implementation we interpolate the following functions
```math
F(x,y) = xy
```
```math
F(x,y) = \exp\left(-\frac{(x+3)^2 + (y+3)^2}{8}\right)
```
```math
F(x,y) = \exp\left(-\sqrt{x^2 + y^2}/4\right) \cos\left(\sqrt{x^2 + y^2}\right)
```
which can be seen in [saddle_plot.svg](./saddle_plot.svg), [gauss_plot.svg](./gauss_plot.svg), and [wave_plot.svg](./wave_plot.svg) respectively. Notice that the interpolation is exact for the first of these. It does reasonably well approximating the gaussian as well but struggles with the oscillations of the third function. Notice also that the sample grid in the second case is not regular, to show that the algorithm works on any rectilinear grid.

### b
This algorithm is also implemented in [bilinear.hpp](./bilinear.hpp) and [bilinear.cpp](./bilinear.cpp) with the signature 
```c++
double bilinear(const std::vector<double>& x,
    const std::vector<double>& y,
    const Matrix<double>& F,
    const std::vector<double>& px, 
    const std::vector<double>& py);
```
where px and py now describe the grid to evaluate the interpolation on. It is then possible to loop through the points in py and px. We always keep track of the rectangle i,j that contained the previous point, because the next must have $i_{next} \geq i$ and $j_{next} \geq j$. Therefore, we simply increase i and j until the next point is contained in the rectangle:
```c++
size_t j = 0;
for (size_t pj = 0; pj < py.size(); ++pj) {
    double pyj = py[pj];
    while (pyj > y[j+1]) j++;
    size_t i = 0;
    for (size_t pi = 0; pi < px.size(); ++pi) {
        double pxi = px[pi];
        while (pxi > x[i+1]) i++;
        ...
```

#### Plots
This method is tested on the functions 
```math
F(x,y) = y-x
```
```math
F(x,y) = \exp\left(-x^2\right) \cos(5y)
```
which can be seen in [plane_plot.svg](./plane_plot.svg) and [gauss_cos_plot](./gauss_cos_plot.svg) respectively.

The timing of the two methods can be seen in [timing_plot.svg](./timing_plot.svg). Here it is clearly seen that the grid version is around twice as fast. Note that in both cases a matrix to store the output is allocated which is in principle unnecessary for the method from part a.

#### Test
Finally, we test that the two methods produce the same results which can be seen in [Out.txt](./Out.txt). They do.
