//Copyright (c) 2019 Mauricio Kugler, Nagoya Institute of Technology

#ifndef SPLINEINTERPOLATIONH
#define SPLINEINTERPOLATIONH

#include "TridiagonalMatrix.h"

class SplineInterpolation1D
{
friend class SplineInterpolation2D;

private:

	unsigned int N;
	float *W;

	TridiagonalMatrix *T;

	float inline spline(float x);

protected:

	void update(float *v);

public:

	SplineInterpolation1D(float *v, unsigned int n);
	~SplineInterpolation1D();

	float interpolate(float x);

};

class SplineInterpolation2D
{
private:

	unsigned int N;
	float *D;

	SplineInterpolation1D **G;
	SplineInterpolation1D *B;

public:

	SplineInterpolation2D(float **v, unsigned int n, unsigned int m);
	~SplineInterpolation2D();

	float interpolate(float x, float y);

};

#endif