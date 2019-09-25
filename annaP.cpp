/*
 * Source File
 * By: Anna Poon
 * Source File main purpose: Start Menu, Animations, Enemy AI
*/
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"


//#define XK_KP_Space 0xff80
/*#include <iostream>
#include <cstdlib>
#include "fonts.h"*/


/*Prototype Functions*/
void startMenu(Rect r, int y, int x, int img_x, int img_y, GLuint imageTexture);
void displayLogo(int x, int y, GLuint texture);
void creditsAnna(Rect r);



/*Function Definitions*/
void startMenu(Rect r, int y, int x, int img_x, int img_y, GLuint imageTexture)
{
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint16(&r, 16, 0x00ff0000, "Press Any Space to Continue");
    ggprint16(&r, 16, 0x00ff0000, 
            "Toggle C for Credits During Gameplay");
	displayLogo(img_x, img_y, imageTexture);
}

void displayLogo(int x, int y, GLuint texture)
{
	
	int width = 80;
	x = 0;
	y = 0;
    glPushMatrix();
    glTranslatef(400, 200, 0);
	
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glTexCoord2f(x, y+.85f);
        glVertex2i(-width,-width);
        glTexCoord2f(x, y);
        glVertex2i(-width,width);
        glTexCoord2f(x+.85f, y);
        glVertex2i(width, width);
        glTexCoord2f(x+.85f, y+.85f);
        glVertex2i(width,-width);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
    
}

void creditsAnna(Rect r)
{
    ggprint8b(&r, 16, 0x00ff0000, "Anna Poon");
}
