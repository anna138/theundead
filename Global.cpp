#include "Global.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>

Global::Global() {
		
		xres = 1250;
		yres = 900;
        memset(keys, 0, 65536);

}
