# Root finding
Project can be built using
```sh
make
```
and cleaned using
```sh
make clean
```
## Requirements
- ```gnuplot``` 

## a
The root-finding algorithm is implemented in [root_finding.hpp](./root_finding.hpp) and [root_finding.cpp](./root_finding.cpp) in the function ```newton```. It uses the QR factorization from the [linear equations homework](../linear_equations/). The Jacobian is calculated using finite-differences in ```Jacobian```. Roots of some example functions can be seen in [Out.txt](./Out.txt) in the colums labeled Newton. For some of the functions multiple $x_0$ are used, either to find multiple root or show that the algorithm converges regardless.

## b
The shooting method for the hydrogen atom is implemented in ```shooting_method_hydrogen```. It uses the ODE solver implemented in the [ODE homework](../ODE/), modified to return only the function value at the endpoint. 

[wavefunc_plot.svg](./wavefunc_plot.svg) shows the determined wavefunctions for various values of $r_{max}$. The approximation is only good when $r_{max}$ is large enough that the exact wavefunction is close to zero (at $r_{max}$).

[scaling_plot.svg](./scaling_plot.svg) shows the convergence of $E_0$ as a function of $r_{max}$, $r_{min}$, acc, and eps. In all cases it converges towards $-1/2$ as expected.

## c
The allocation of the Jacobian is removed by making ```Jacobian``` take a matrix to store the result into as an argument. Note however, that the QR decomposition is still allocated each time, which could be avoided by modifying the QR class.

Quadratic line-search is implemented in ```newton_qls```. Results can be seen in [Out.txt](./Out.txt) in the column QLS. For most functions it uses around the same number of evaluations, however it does significantly better for the Rosenbrock function.
