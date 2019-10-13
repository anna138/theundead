#pragma once
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0

typedef float Vec[3];

class Villain {
public:
	unsigned int villainImageTexture; 
	Vec dir;
	Vec pos;
	Vec vel;
	float angle;
	float color[3];
	Villain();
};
