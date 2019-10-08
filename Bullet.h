#pragma once
#include <ctime>

typedef float Vec[3];

class Bullet {
public:
	Vec pos;
	Vec vel;
	float color[3];
	struct timespec time;

};
