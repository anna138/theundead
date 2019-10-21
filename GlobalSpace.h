#pragma once
#include <GL/glx.h>
#include "Global.h"

//define all global variables here that go
namespace gvars{
    typedef float Vec[3];
    typedef float Matrix[4][4];
    extern int img_x;
    extern int img_y;
    extern unsigned int imageTexture; 
    extern unsigned int startMenuTexture; 
    extern unsigned int trooperImageTexture; 
    extern unsigned int villainImageTexture;
    extern unsigned int titleImageTexture; 
    extern const float timeslice;
    extern const float gravity;
    extern const int MAX_BULLETS;
    extern const float MINIMUM_ASTEROID_SIZE;
    extern const double physicsRate;
    extern const double oobillion;
    extern int started;
    extern int doneStart;
    extern int changeColor;
    extern int logo;
    extern Global gl;
    extern int yourCurrentScore;
}