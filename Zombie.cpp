#include "Zombie.h"
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0


Zombie::Zombie() {
	VecZero(dir);
	pos[0] = rand()%500;
	pos[1] = rand()%200;
	pos[2] = 0.0;
	VecZero(vel);
	angle = 0.0;
	color[0] = color[1] = color[2] = 1.0;
}
