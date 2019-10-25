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
    int credits = 0;
    int highScore = 0;
    int gameOver = 0;
    int grabHighScores = 0;
    int changed = 0;
    Global gl;
    int yourCurrentScore=0;
    int fireColors[5][3] = {{128, 0, 0}, 
    {255, 80, 66}, {255, 0, 0}, {245, 18, 0}, {255, 174, 66}};
    int lightningColors[2][3] = {{0, 0, 255}, {255, 255, 255}};
} // namespace GVars
