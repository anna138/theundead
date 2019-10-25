#pragma once
#include "GlobalSpace.h"

using gvars::Vec;

class Villain {
public:
	unsigned int villainImageTexture; 
	Vec dir;
	Vec pos;
	Vec vel;
	float angle;
	float color[3];
	float size[2];
	Villain();
};
