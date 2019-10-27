#pragma once
#include "GlobalSpace.h"

using gvars::Vec;

class Skull {
public:
	unsigned int skullImageTexture; 
	Vec dir;
	Vec pos;
	Vec vel;
	float angle;
	float color[3];
	float size[2];
	Skull();
};
