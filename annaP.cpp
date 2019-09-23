//Source File: Anna Poon
//
//#define XK_KP_Space 0xff80
#include <iostream>
#include <cstdlib>
#include "fonts.h"
/*Prototype Functions*/

/*Function Definitions*/
void startMenu(Rect r, int y, int x)
{
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint16(&r, 16, 0x00ff0000, "Press Any Space to Continue");
/*
    if(key) {
            *start = true;
    }
*/   
}

void creditsAnna(Rect r)
{
    ggprint8b(&r, 16, 0x00ff0000, "Anna Poon");
}
