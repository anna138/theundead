#include "Skull.h"
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0


Skull::Skull() {
	VecZero(dir);
	// pos[0] = (Flt)(gl.xres/2);
	// pos[1] = (Flt)(gl.yres/2);
	pos[0] = ((2.0f*((float)rand()/RAND_MAX))-1.0f)*500.0f;
	pos[1] = ((2.0f*((float)rand()/RAND_MAX))-1.0f)*200.0f;
	pos[2] = 0.0;
	VecZero(vel);
	skullImageTexture = gvars::gl.skullTexture;
	angle = 0.0;
	color[0] = color[1] = color[2] = 1.0;
}
