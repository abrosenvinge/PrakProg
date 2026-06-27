# Linear equations
Project can be built using
```sh
make
```
and cleaned using
```sh
make clean
```

## a
Matrix and Vector are implemented in [../lib/matrix.hpp](../lib/matrix.hpp).

[qr.hpp](qr.hpp) contains the implementation of QR factorization in the class QR. [Out.txt](Out.txt) shows the decomposition of a matrix $A$. It can be seen that $R$ is upper triangular, that $QR=A$, and $Q^T Q = I$. Additionally shown is the solution of the linear equation $Ax=b$ for some $b$. $b$ is determined by constructing a random vector $x_\text{correct}$ and setting $b=Ax_\text{correct}$. This way the determined $x$ can be compared to the correct solution. Also shown is the determinant of $A$.

## b
[Out.txt](Out.txt) also shows the calculation of the inverse of some square matrix $B$. It is shown that $B^{-1}B = B B^{-1} = I$.

## c
Calling ```main``` with the ```--time``` flag calculates only the QR decomposition. The time taken to do this for a random $N\times N$ matrix as a function of $N$ can be seen in [plot.svg](plot.svg). Additionally, a fit of $aN^3$ can be seen, which matches nicely.
