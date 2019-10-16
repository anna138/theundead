#pragma once
#include "GlobalSpace.h"

using gvars::Vec;

class Trooper {
public:
	unsigned int trooperImageTexture; 
	Vec dir;
	Vec pos;
	Vec vel;
	float angle;
	float color[3];
	Trooper();
};
