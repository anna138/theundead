#include "GlobalSpace.h"
#include <cstdlib>

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
    int changeColor = 0;
    Global gl;
    int yourCurrentScore = 0;
    //int bullet_position = rand() % 75 + 35;
    int fireColors[5][3] = {{128, 0, 0}, 
        {255, 80, 66}, {255, 0, 0}, {245, 18, 0}, {255, 174, 66}};
    int lightningColors[2][3] = {{0, 0, 255}, {255, 255, 255}};
    int grassColors[3][3] = {{5, 115, 52}, {30, 120, 62}, {104, 120, 30}};
    int waterColors[2][3] = {{0, 255, 0}, {0, 128, 255}}; 
    GameState state;
} // namespace GVars
