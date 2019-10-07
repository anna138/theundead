#include "Villain.h"
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0


Villain::Villain() {
		VecZero(dir);
		// pos[0] = (Flt)(gl.xres/2);
		// pos[1] = (Flt)(gl.yres/2);
		pos[0] = 0.0f;
		pos[1] = 0.0f;
		pos[2] = 0.0f;
		VecZero(vel);
		angle = 0.0;
		color[0] = color[1] = color[2] = 1.0;
}
