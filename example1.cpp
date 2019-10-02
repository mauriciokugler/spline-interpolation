//Copyright (c) 2019 Mauricio Kugler, Nagoya Institute of Technology

#include "SplineInterpolation.h"
#include <stdlib.h>

int main(void)
{
	unsigned int n = 10;

	float *v = new float[n];

	for(unsigned int i=0;i<n;i++) {
		v[i] = ((float)rand()/RAND_MAX)*2-1;
	}

	SplineInterpolation1D *SPL = new SplineInterpolation1D(v,n);

	float x = ((float)rand()/RAND_MAX) * (n-1);

	float u = SPL->interpolate(x);

	delete SPL;

	return 0;
}