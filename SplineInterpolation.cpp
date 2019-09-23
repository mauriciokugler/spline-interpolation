//Copyright (c) 2019 Mauricio Kugler, Nagoya Institute of Technology

#include "SplineInterpolation.h"
#include <math.h>

const float s = (float)1/6;

SplineInterpolation1D::SplineInterpolation1D(float *x, float *v, unsigned int n)
{
	N = n;
	C = x;

	T = new TridiagonalMatrix(N, s, 4*s, s);
	W = T->system(v);
}

SplineInterpolation1D::~SplineInterpolation1D()
{
	delete T;
}

float SplineInterpolation1D::interpolate(float x)
{
	int p = (int)fmin(fmax(roundf(x), 2), N-3);

	float h = 0;
	for(int j=p-2;j<=p+2;j++) {
		h += W[j] * spline(x-C[j]);
	}

	return(h);
}

void SplineInterpolation1D::update(float *v)
{
	W = T->system(v);
}

float inline SplineInterpolation1D::spline(float x)
{
	float y = fabsf(x);

	if(y<1) {
		float f1 = 1-y;
		float f2 = f1*f1;
		float f3 = f2*f1;

		float h = 0.5f*(f1+f2-f3) + s;
		
		return(h);
	}
	else if(y<2) {
		float f1 = 2-y;
		
		float h = f1*f1*f1*s;

		return(h);
	}
	else {
		return(0);
	}
}

SplineInterpolation2D::SplineInterpolation2D(float *x, float *y, float **v, unsigned int n, unsigned int m)
{
	N = n;

	C = y;

	G = new SplineInterpolation1D*[N];
	D = new float[N];

	B = NULL;

	#pragma omp parallel for
	for(int i=0;i<(int)N;i++) {
		G[i] = new SplineInterpolation1D(x, v[i], m);
	}
}

SplineInterpolation2D::~SplineInterpolation2D()
{
	for(unsigned int i=0;i<N;i++) delete G[i];
	delete[] G;
	
	if(B!=NULL) delete B;
	delete[] D;
}

float SplineInterpolation2D::interpolate(float x, float y)
{
	#pragma omp parallel for
	for(int i=0;i<(int)N;i++) {
		D[i] = G[i]->interpolate(x);
	}

	if(B==NULL) B = new SplineInterpolation1D(C,D,N);
	else B->update(D);

	float h = B->interpolate(y);

	return(h);
}