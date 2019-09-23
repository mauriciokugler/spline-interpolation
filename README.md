# C++ B-Spline Interpolation

Yet another C++ implementation for [B-spline](https://en.wikipedia.org/wiki/B-spline) [interpolation](https://en.wikipedia.org/wiki/Spline_interpolation). These classes provides simple and efficient methods for 1D & 2D data B-spline interpolation in regular grids.

## Usage

The 1D & 2D B-spline interpolation objects can be instantiated with the following constructors:

```C++
SplineInterpolation1D(float *x, float *v, unsigned int n);
SplineInterpolation2D(float *x, float *y, float **v, unsigned int n, unsigned int m);
```

For the 1D interpolation, the vector *x* contain *n* sample points, while the array *v* contains the corresponding values. For the 2D interpolation, vectors *x* and *y*, containing the 2-axis sampling coordinates, have, respectively, *n* and *m* points, while the array of corresponding values *v* has size *n*&#215;*m*.

The value of query points *u(x)* and *u(x,y)* are calculated with the following methods:   

```C++
float interpolate(float x);
float interpolate(float x, float y);
```

The 2D interpolation calculates and stores the B-spline coefficients of the *x*-axis, solving a single linear system for each new query point. 

## Example

The `example1.cpp` file contains a simple example of a 1D interpolation of a random vector:

```C++
#include "SplineInterpolation.h"
#include <stdlib.h>

int main(void)
{
  unsigned int n = 10;

  float *y = new float[n];
  float *v = new float[n];

  for(unsigned int i=0;i<n;i++) {
    y[i] = (float)i;
    v[i] = ((float)rand()/RAND_MAX)*2-1;
  }

  SplineInterpolation1D *SPL = new SplineInterpolation1D(y,v,n);

  float x = ((float)rand()/RAND_MAX) * (n-1);

  float u = SPL->interpolate(x);

  delete SPL;

  return 0;
}
```

The following plot shows the interpolated curve calculated from 10 initial points:

<img src="bspline_1d.png" alt="drawing" width="500"/>

The `example2.cpp` file contains a simple example of a 2D interpolation of a random surface:

```C++
#include "SplineInterpolation.h"
#include <stdlib.h>
#include <omp.h>

int main(void)
{
  unsigned int n = 30;
  unsigned int m = 20;

  float *x = new float[n];
  float *y = new float[m];

  for(unsigned int i=0;i<n;i++) x[i] = (float)i;
  for(unsigned int j=0;j<m;j++) y[j] = (float)j;

  float **v = new float*[n];
  for(unsigned int i=0;i<n;i++) {
    v[i] = new float[m];
    for(unsigned int j=0;j<m;j++) {
      v[i][j] = ((float)rand()/RAND_MAX)*2-1;
    }
  }

  SplineInterpolation2D *SPL = new SplineInterpolation2D(x, y, v, n, m);

  float p = ((float)rand()/RAND_MAX) * (n-1);
  float q = ((float)rand()/RAND_MAX) * (m-1);

  float u = SPL->interpolate(p, q);

  delete SPL;

  return 0;
}
```

The following plot shows the interpolated surface calculated from a grid of 20&#215;30 initial points:

<img src="bspline_2d.png" alt="drawing" width="500"/>

The [OpenMP](https://en.wikipedia.org/wiki/OpenMP) parallelization is optional, but has critical impact on the code's performance. 

## Citing

```TeX
@MISC{Kugler2019,
  author = "Mauricio Kugler",
  title = "C++ B-Spline Interpolation",
  year = "2019",
  url = "https://github.com/mauriciokugler/spline-interpolation",
  note = "Version 1.0.0"
}
```

## License

This project is licensed under the [MIT License](LICENSE).