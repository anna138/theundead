#include "Skull.h"
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0


Skull::Skull() {
	VecZero(dir);
	// pos[0] = (Flt)(gl.xres/2);
	// pos[1] = (Flt)(gl.yres/2);
	pos[0] = 0.0;
	pos[1] = 200;
	pos[2] = 0.0;
	VecZero(vel);
	angle = 0.0;
	color[0] = color[1] = color[2] = 1.0;
}
