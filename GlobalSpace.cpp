#include "GlobalSpace.h"

namespace gvars{
    typedef float Vec[3];
    typedef float Matrix[4][4];
    int img_x;
    int img_y;
    unsigned int imageTexture; 
    unsigned int startMenuTexture; 
    unsigned int trooperImageTexture; 
    unsigned int villainImageTexture; 
    unsigned int titleImageTexture; 
    const float timeslice = 1.0f;
    const float gravity = -0.2f;
    const int MAX_BULLETS = 11;
    const float MINIMUM_ASTEROID_SIZE = 60.0;
    const double physicsRate = 1.0 / 60.0;
    const double oobillion = 1.0 / 1e9;
    int started = 0;
    int doneStart = 0;
    int changeColor=0;
    int logo = 0;
    Global gl;
    int yourCurrentScore=0;

} // namespace GVars
