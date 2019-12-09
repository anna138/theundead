#include "GlobalSpace.h"
#include <cstdlib>

namespace gvars{
    typedef float Vec[3];
    typedef float Matrix[4][4];
    int img_x;
    int img_y;
    int attack = 0;
    int xres = 1920;
    int yres = 1080;
    unsigned int imageTexture; 
    unsigned int startMenuTexture; 
    unsigned int trooperImageTexture; 
    unsigned int villainImageTexture; 
    unsigned int skullImageTexture;
    unsigned int zombieImageTexture;
    unsigned int titleImageTexture;
    unsigned int fireImageTexture; 
    unsigned int grassImageTexture;
    unsigned int waterImageTexture;
    unsigned int lightningImageTexture; 
    const float timeslice = 1.0f;
    const float gravity = -0.2f;
    const int MAX_BULLETS = 20;
    const int MAX_SKULLS = 2;
    const float MINIMUM_ASTEROID_SIZE = 60.0;
    const double physicsRate = 1.0 / 60.0;
    const double oobillion = 1.0 / 1e9;
    int changeColor = 0;
    Global gl;
    int yourCurrentScore=0;
    int array[11];
    int arrayVine[11];


    int fireColors[5][3] = {{128, 0, 0}, 
        {255, 80, 66}, {255, 0, 0}, {245, 18, 0}, {255, 174, 66}};
    int lightningColors[2][3] = {{0, 0, 255}, {255, 255, 255}};
    int grassColors[3][3] = {{5, 115, 52}, {30, 120, 62}, {104, 120, 30}};
    int waterColors[3][3] = {{0, 0, 255}, {77, 166, 255}, {255, 255, 255}}; 
    GameState state;
    int movex = 0;
    int movey = 0;
    int fireArray[3] = {100, 150, 0};
    int sizeX = 200;
    int sizeY = 200;
    int playerdir = 0;
    MainCharacter hero;
    int arrow_keys[4] = {0, 0, 0, 0};

    TileParser tp("./images/level1/level1.tmx");
    Texture map("./images/level1/level1_0.png");
    Texture map_1("./images/level1/level1_1.png");
    Texture map2("./images/level2/level2_0.png");
    Texture map2_1("./images/level2/level2_1.png");


} // namespace GVars
