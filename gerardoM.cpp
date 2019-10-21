
/*Four steps of Debugging
 *1)Recognize that a bug exists
 *2)Isolate the source of the bug
 *3)Identify the cause of the bug
 * 4)Determine a fix for the bug
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
#include <math.h>
#define DEG2RAD 3.14159/180.0
void drawSquare (int width, int height, int dirX, int dirY, int choice);
void boxText(Rect r);
extern void postScores(char*);
void creditsGerardo(Rect r)
{
    ggprint8b(&r, 16, 0x00004C00, "Gerardo Martinez Flores");
}
void makeButton(int x, int y, int dirX, int dirY)
{
    int width = x/2;
    int height = y/6;
    int choice=3;
    int posx[3]={-300,0,300};
    int posy[3]={-185,-185,-185};
    glPushMatrix();

    glBegin(GL_QUADS);
    glColor3f(0.5,0.0,0.0);
    for(int i=0;i<3;i++) {
        dirX = posx[i];
        dirY = posy[i];
        drawSquare(width, height, dirX, dirY, choice);
    }
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0,0.0,0.0);
    for(int i=0;i<3;i++) {
        dirX = posx[i];
        dirY = posy[i];
        drawSquare(width - 3, height - 3, dirX, dirY, choice);
    }
    glEnd();

    glPopMatrix();
}

void changeButtonColor( int y, int x ,int dirX, int dirY, int &doneStart) {

    int width = x/2;
    int height = y/6;
    int choice=3;
    int posx[3]={-300,0,300};
    int posy[3]={-185,-185,-185};
    for(int i=0;i<3;i++) {
        dirX=posx[i];
        dirY=posy[i];
        glColor3f(0.0,1.0,1.0);
        drawSquare(width,height,dirX,dirY,choice);
    }
    doneStart = 0;
}
bool posted = false;
void displaycurrentscore(Rect r, int h, int w, int bestScore,int yourScore){
    r.bot=h-h;
    r.left=w-w-25;
    r.center=0;
    int temp = rand();
    ggprint16(&r, 16, 0x003B8B68, "Game Over\n");
    ggprint16(&r, 16, 0x003B8B68, "Your score:%d \n", yourScore);
    ggprint16(&r, 16, 0x003B8B68, "Best score:%d \n", bestScore);
    ggprint16(&r, 16, 0x003B8B68, "\nTap to restart\n");
    char pn [100];

    sprintf(pn, "~mbal/3350/lab7/scores.php?name=%s&score=%d",getenv("USER"),temp%10); 
    if(!posted){
        posted = True;
        postScores(pn);
    }
}
void boxText(Rect r) {
    std::string boxText[3]={"Characters","Start Game","Credits"};
    int posx[3]={-330, -30, 282};
    int posy[3]={-190,-190,-190};
    for(int i=0;i<3;i++) {
    r.left=posx[i];
    r.bot=posy[i];
    r.center=0;
    ggprint8b(&r,16,0x002E8B57, boxText[i].c_str());
    }

}
void drawSquare(int x, int y, int dirX, int dirY, int choice) {
    int w=x;
    int h=y;
    if(choice==3) {
        glBegin(GL_QUADS);
        glVertex2i(w+dirX, -h+dirY);
        glPushMatrix();

        glVertex2i(w+dirX, h+dirY);
        glVertex2i(-w+dirX, h+dirY);
        glVertex2i(-w+dirX, -h+dirY);
        glEnd();
        glPopMatrix();
    }

}

void drawLine() {
    glPushMatrix();
    for(int i=0;i<10;i++) {
    glLineWidth(7);
    glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(100,200);
        glVertex2f(200,200);
        glEnd();
        glLineWidth(2);
        glBegin(GL_LINES);
        glColor3f(1.0,1.0,1.0);
        glVertex2f(100,200);
        glVertex2f(200,200);
    }
    glEnd();
    glPopMatrix();
    glLineWidth(1);
}
void BulletColor(int n){
    switch(n)
    {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
    }

}
