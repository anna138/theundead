#include "Zombie.h"
#include <iostream>
#include "GlobalSpace.h"
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0


Zombie::Zombie() {
	VecZero(dir);
	pos[0] = ((2.0f*((float)rand()/RAND_MAX))-1.0f)*500.0f;
	pos[1] = ((2.0f*((float)rand()/RAND_MAX))-1.0f)*200.0f;
	pos[2] = 0.0;
	zombieImageTexture = gvars::gl.zombieTexture;
	VecZero(vel);
	angle = 0.0;
	color[0] = color[1] = color[2] = 1.0;
}
