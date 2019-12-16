#pragma once
//macros functions
#define rnd() (((float)rand()) / (float)RAND_MAX)
//#define random(a) (((rand()) % (a))
#define VecZero(v) (v)[0] = 0.0, (v)[1] = 0.0, (v)[2] = 0.0
#define MakeVector(x, y, z, v) (v)[0] = (x), (v)[1] = (y), (v)[2] = (z)
#define VecCopy(a, b) \
	(b)[0] = (a)[0];  \
	(b)[1] = (a)[1];  \
	(b)[2] = (a)[2]
#define VecDot(a, b) ((a)[0] * (b)[0] + (a)[1] * (b)[1])
#define VecSub(a, b, c)       \
	(c)[0] = (a)[0] - (b)[0]; \
	(c)[1] = (a)[1] - (b)[1]; \
	(c)[2] = (a)[2] - (b)[2]
#define VecMag(a) (sqrt(a[0]*a[0]+a[1]*a[1]))
//macros constants
#define PI 3.141592653589793
#define ALPHA 1