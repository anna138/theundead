#pragma once
#include <GL/glx.h>
#include "Global.h"

//define all global variables here that go
namespace gvars{
    typedef float Vec[3];
    typedef float Matrix[4][4];
    extern int img_x;
    extern int img_y;
    extern GLuint imageTexture; 
    extern GLuint startMenuTexture; 
    extern GLuint trooperImageTexture; 
    extern GLuint villainImageTexture; 
    extern const float timeslice;
    extern const float gravity;
    extern const int MAX_BULLETS;
    extern const float MINIMUM_ASTEROID_SIZE;
    extern const double physicsRate;
    extern const double oobillion;
    extern int started;
    extern int doneStart;
    extern int changeColor;
    extern Global gl;
    extern int yourCurrentScore;
}