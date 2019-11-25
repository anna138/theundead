#pragma once
#include <GL/glx.h>
#include "MainCharacter.h"
#include "Global.h"

//define all global variables here that go
namespace gvars{
    typedef float Vec[3];
    typedef float Matrix[4][4];
    extern int img_x;
    extern int img_y;
    extern int attack;
    extern int xres;
    extern int yres;
    extern unsigned int imageTexture; 
    extern unsigned int startMenuTexture; 
    extern unsigned int trooperImageTexture; 
    extern unsigned int skullImageTexture;
    extern unsigned int zombieImageTexture;
    extern unsigned int titleImageTexture; 
    extern unsigned int fireImageTexture; 
    extern unsigned int grassImageTexture;
    extern unsigned int waterImageTexture;
    extern unsigned int lightningImageTexture; 
    extern const float timeslice;
    extern const float gravity;
    extern const int MAX_BULLETS;
    extern const int MAX_SKULLS;
    extern const float MINIMUM_ASTEROID_SIZE;
    extern const double physicsRate;
    extern const double oobillion;
    extern int array[11];
    extern int arrayVine[11];
    extern int changeColor;
    extern Global gl;
    extern int yourCurrentScore;
    extern int fireColors[][3];
    extern int waterColors[][3];
    extern int grassColors[][3];
    extern int lightningColors[][3];
    extern int bullet_position;
    extern int movex;
    extern int movey;
    extern int sizeX;
    extern int sizeY;
    extern int playerdir;
    extern MainCharacter hero;
    enum class GameState{
        startup,
        menu,
        game,
        credits,
        highscores,
        endgamescore,
        end
    };
    enum class Direction{
        S,
        SW,
        W,
        NW,
        N,
        NE,
        E,
        SE,
        end
    };
    
    extern GameState state;
}

