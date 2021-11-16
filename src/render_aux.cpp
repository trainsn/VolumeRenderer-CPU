////////////////////////////////////////////////////////
//
//   Han-Wei Shen 
//   01/25/1996
//

#ifndef RENDER_AUX
#define RENDER_AUX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>

#include "render.h"
#include "Map.h"

//////////////////////////////////////////
//
//  Some auxilary functions 
//
//

// Vector Dot product
//
float Dot(uvw *v1, uvw *v2)
{
	return (v1->u*v2->u + v1->v*v2->v + v1->w*v2->w);
}

//////////////////////////////////
//
// Normalize a vector, avoiding work if possible 
// (e.g. already normalized, or zero vector)
//
void Normalize(uvw *v)
{
	float magn = Dot(v, v);
	if (magn > EPS && fabs(magn - 1.0) > EPS)
	{
		magn = sqrt(magn);
		v->u /= magn;
		v->v /= magn;
		v->w /= magn;
	}
}
///////////////////////////////////
//
// Clamp a value  to ensure it lies between min and max
//
float clamp(float value, float min, float max)
{
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

///////////////////////////////////
//
// Interpolate t of the way between y0 and y1.
//
float lerp(float t, float y0, float y1)
{
	return y0 + t * (y1 - y0);
}

///////////////////////////////////
//
// Raise a float to an integer power (for phong highlight) 
//
float ipow(float x, int n)
{
	int i;
	float result = 1, factor = x;

	// loop through bits of n to give O(log(n)) time bound 
	for (i = 1; i <= n; i <<= 1)
	{
		if (i & n) result *= factor;   // i is part of n's base 2 representation
		factor *= factor;              // factor is now X^( log(i)+1)
	}
	return result;
}

#endif
