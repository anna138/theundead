#pragma once
#include "GlobalSpace.h"

using gvars::Vec;

class Zombie {
public:
	unsigned int zombieImageTexture; 
	Vec dir;
	Vec pos;
	Vec vel;
	float angle;
	float color[3];
	float size[2];
	Zombie();
};
