//Source File: Anna Poon
//
//#define XK_KP_Space 0xff80

#include "fonts.h"
/*Prototype Functions*/

/*Function Definitions*/
void startMenu(Rect r, char key, int y, int x, bool *start)
{
    r.bot = y - 200;
    r.left = x - 50;
    r.center = 0;

    ggprint16(&r, 16, 0x00ff0000, "Press Any Space to Continue");
    
    if(key) {
        *start = true;
    }
}

void creditsAnna(Rect r)
{
    ggprint8b(&r, 16, 0x00ff0000, "Anna Poon");
}
