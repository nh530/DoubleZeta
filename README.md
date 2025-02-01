# DoubleZeta
Multi-threading math. 
For prototyping purposes only. Not a substitute for BLAS. 
Matrix addition speed is comparable to numpy


# Build Instructions
1. Navigate to the build directory. If there is no build directory, make one if the repository root.
2. Enter the following command in a terminal, `cmake ../` , followed by `cmake --build ./`.

# Python installation
Navigate to the root folder and run `python ./setup.py install`.

# Example
```
import doublezeta

a = doublezeta.CpyMatrix(2, 2, 1, 'float')
b = doublezeta.CpyMatrix(2, 2, 1.5, 'float')
a.mult(b)
a.add(b)
```
