# EVD
Project can be built using
```sh
make
```
and cleaned using
```sh
make clean
```
Making [timing_plot.svg](./timing_plot.svg) may take some time.

## Requirements
- ```gnuplot``` 
- ```GNU time``` available at ```/usr/bin/time```

## a
[evd.hpp](./evd.hpp) contains the implementation of the eigenvalue decompositon in the class ```EVD```. This class contains the static methods ```Jtimes``` and ```timesJ``` which multiply by the Jacobi rotation matrix. The EVD for this part is implemented in the method ```construct_unoptimized``` which is called by the ```EVD``` constructor.

Tests of the algorithm can be seen in [Out.txt](./Out.txt). Here it is seen that $V^T A V = D$, $V D V^T = A$, and $V V^T = V^T V = I$.

## b
Calculated and exact wavefunctions can be seen plotted in [wavefunc_plot.svg](./wavefunc_plot.svg). It can be seen that the approximation is good for $k=1$ but gets worse for higher $k$. Some exact wavefunctions have been multiplied by $-1$ such that the phase matches the calculated wavefunction.

[dr_plot.svg](./dr_plot.svg) and [rmax_plot.svg](./rmax_plot.svg) show the convergence of the computed $E_0$ as a function of $\Delta r$ and $r_{max}$ respectively. It can be seen in both cases that $E_0$ converges towards $-0.5$ as expected.

## c
The ```construct_optimized``` method of ```EVD``` calculates the EVD of a matrix but uses only the upper triangle of the matrix when calculating the Jacobi rotations. This is done using the helper class ```UpperTriangleHelper``` and the method ```jacobi_rot```. A plot of the time taken to compute the EVD of an $N\times N$ matrix as a function of $N$ can be seen in [timing_plot.svg](./timing_plot.svg). The optimized version is around twice as fast as expected.
