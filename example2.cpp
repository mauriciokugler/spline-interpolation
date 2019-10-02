//Copyright (c) 2019 Mauricio Kugler, Nagoya Institute of Technology

#include "SplineInterpolation.h"
#include <stdlib.h>
#include <omp.h>

int main(void)
{
	unsigned int n = 30;
	unsigned int m = 20;

	float **v = new float*[n];
	for(unsigned int i=0;i<n;i++) {
		v[i] = new float[m];
		for(unsigned int j=0;j<m;j++) {
			v[i][j] = ((float)rand()/RAND_MAX)*2-1;
		}
	}

	SplineInterpolation2D *SPL = new SplineInterpolation2D(v, n, m);

	float x = ((float)rand()/RAND_MAX) * (n-1);
	float y = ((float)rand()/RAND_MAX) * (m-1);

	float u = SPL->interpolate(x,y);

	delete SPL;

	return 0;
}