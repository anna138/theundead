#pragma once
#include <ctime>

typedef float Vec[3];

class Bullet {
public:
	Vec pos;
	Vec vel;
	int color[3];
	int angle;
	struct timespec time;
	int row;
	int type;
};
