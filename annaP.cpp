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
void displayLogo(int w, int h, int x, int y, GLuint texture);
void creditsAnna(Rect r);



/*Function Definitions*/
void startMenu(Rect r, int y, int x, int img_x, int img_y, GLuint imageTexture)
{
	img_x = img_y + imageTexture;
	img_y = 0 + img_x;
	imageTexture =0;
    	r.bot = 0-(y/3);
    	r.left = 0-(x/7);
    	r.center = 0;
    	ggprint16(&r, 16, 0x00ff0000, "Press Space to Continue");
    	ggprint16(&r, 16, 0x00ff0000, 
            "Press Space + C for Credits During Gameplay");
}

void displayLogo(int w, int h, int x, int y, GLuint texture)
{
	
	int width = w/2;
	int height = h/2;
    	glPushMatrix();
	//glTranslatef(,0,0);
    	glColor3f(1.0,1.0,1.0);
    	glBindTexture(GL_TEXTURE_2D, texture);
    	glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2i(-width,-height);
        glTexCoord2f(0, 1);
        glVertex2i(-width,height);
        glTexCoord2f(1, 1);
        glVertex2i(width, height);
        glTexCoord2f(1,0);
        glVertex2i(width,-height);
    	glEnd();
    	glPopMatrix();
    	glBindTexture(GL_TEXTURE_2D, 0);
    	glDisable(GL_ALPHA_TEST);
    
}

void creditsAnna(Rect r)
{
    	ggprint8b(&r, 16, 0x00004C00, "Anna Poon");
}
