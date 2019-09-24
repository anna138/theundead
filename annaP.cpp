//Source File: Anna Poon
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
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
	displayLogo(img_x, img_y, imageTexture);
/*
    if(key) {
            *start = true;
    }
*/   
}

void displayLogo(int x, int y, GLuint texture)
{
	
	int width = 100;
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
        glTexCoord2f(x, y+.5);
        glVertex2i(-width,-width);
        glTexCoord2f(x, y);
        glVertex2i(-width,width);
        glTexCoord2f(x+.125, y);
        glVertex2i(width, width);
        glTexCoord2f(x+.125, y+.5);
        glVertex2i(width,-width);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
    
}

/*SAMPLE CODE ANNA
void displayLogo(int x, int y, float tx, float ty, GLuint texid){
    int width = 40; 
    static float fx = 0.0f;
    static float angle = 0.0f;

    angle += 0.01;
    fx =sin(angle)
    y = y - 40;

    glPushMatrix();
    glTranslatef(x + (int)(fx*30,0), y, 0);
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D,texid);
    glBegin(GL_QUADS);
        glTexCoord2f(tx, ty+.5);
        glVertex2i(-wid,-wid);
        glTexCoord2f(tx, ty);
        glVertex2i(-wid,wid);
        glTexCoord2f(tx+.125, ty);
        glVertex2i(wid, wid);
        glTexCoord2f(tx+.125, ty+.5);
        glVertex2i(wid,-wid);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}
*/
void creditsAnna(Rect r)
{
    ggprint8b(&r, 16, 0x00ff0000, "Anna Poon");
}
