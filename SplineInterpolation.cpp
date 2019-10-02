//Copyright (c) 2019 Mauricio Kugler, Nagoya Institute of Technology

#include "SplineInterpolation.h"
#include <math.h>

const float s = (float)1/6;
const unsigned int r = 4;
const unsigned int R = 2*r;

SplineInterpolation1D::SplineInterpolation1D(float *v, unsigned int n)
{
	N = n;

	T = new TridiagonalMatrix(N, s, 4*s, s);
	W = T->system(v);
}

SplineInterpolation1D::~SplineInterpolation1D()
{
	delete T;
}

float SplineInterpolation1D::interpolate(float x)
{
	int p = (int)fmin(fmax(roundf(x),2),N-3);

	float h = 0;
	for(int j=p-2;j<=p+2;j++) {
		h += W[j] * spline(x-j);
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

SplineInterpolation2D::SplineInterpolation2D(float **v, unsigned int n, unsigned int m)
{
	N = n;

	G = new SplineInterpolation1D*[N];
	D = new float[R];

	B = NULL;

	#pragma omp parallel for
	for(int i=0;i<(int)N;i++) {
		G[i] = new SplineInterpolation1D(v[i], m);
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
	unsigned int j = (unsigned int)fmaxf(0,floorf(x)-r+1);
	if(R>N-j || j>N) j = N-R;
	
	#pragma omp parallel for
	for(int i=0;i<(int)R;i++) {
		D[i] = G[i+j]->interpolate(y);
	}

	if(B==NULL) B = new SplineInterpolation1D(D,R);
	else B->update(D);

	float h = B->interpolate(x-j);

	return(h);
}