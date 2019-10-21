#pragma once
#include <ctime>

typedef float Vec[3];

class Bullet {
public:
	Vec pos;
	Vec vel;
	int color[3];
	struct timespec time;
	int row;

};
