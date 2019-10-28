
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
#include "GlobalSpace.h"
#define DEG2RAD 3.14159/180.0
//void drawlight(int x1, int y1,int x2,int y2,int displace);
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
    int posx[3]={-(x / 2 +  x / 2 + x / 2), 0, (x / 2 +  x / 2 + x / 2)};
    int posy[3]={-(y / 2 + y / 3 + y / 7 + y / 22) + y / 13, -(y / 2 + y / 3 + y / 7 + y / 22) + y / 13, -(y / 2 + y / 3 + y / 7 + y / 22)  + y / 13};
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
    glColor3f(0.7,0.13,0.13);
    for(int i=0;i<3;i++) {
        dirX = posx[i];
        dirY = posy[i];
        drawSquare(width - 3, height - 3, dirX, dirY, choice);
    }
    glEnd();

    glPopMatrix();
}

void changeButtonColor( int y, int x ,int dirX, int dirY) {

    int width = x/2;
    int height = y/6;
    int choice=3;
    int posx[3]={-300,0,300};
    int posy[3]={-185,-185,-185};
    for(int i=0;i<3;i++) {
        dirX=posx[i];
        dirY=posy[i];
        glColor3f(0.0,1.0,1.0);
        drawSquare(width - 3, height - 3,dirX,dirY,choice);
    }
}
void displaycurrentscore(Rect r, int h, int w, int bestScore,int yourScore){
    r.bot=h-h;
    r.left=w-w-25;
    r.center=0;
    ggprint16(&r, 16, 0x003B8B68, "Game Over\n");
    ggprint16(&r, 16, 0x003B8B68, "Your score:%d \n", yourScore);
    ggprint16(&r, 16, 0x003B8B68, "Best score:%d \n", bestScore);
    ggprint16(&r, 16, 0x003B8B68, "\nTap to restart\n");
    char pn [100];

    sprintf(pn, "~mbal/3350/lab7/scores.php?name=%s&score=%d",getenv("USER"),yourScore); 
    postScores(pn);
}
void boxText(Rect r) {
    std::string boxText[3]={"Characters","Start Game","Credits"};
    int posx[3]={-330, -30, 282};
    int posy[3]={-190,-190,-190};
    for(int i=0;i<3;i++) {
        r.left=posx[i];
        r.bot=posy[i];
        r.center=0;
        ggprint8b(&r,16,0x00cefdce, boxText[i].c_str());
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
void populateWithRand(int * array, unsigned int size, int start, int end)
{
    for(unsigned int i = 0; i < size;i++){
        int randsign = rand()%2;
        int randXnum = randsign ? rand()%(end-start)+start: 
                        -(rand()%(end-start)+start);
        array[i] = randXnum;
    }
}
void drawLine() {
    glPushMatrix();
    int counter=0;
    for(int i=0;i<10;i++) {
        // int randsign = rand()%2;
        // int randXnum = randsign ? rand()%10: -(rand()%10);
        glLineWidth(7);
        glBegin(GL_LINE_LOOP);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(100+gvars::array[counter],200-(50*i));
        glVertex2f(100+gvars::array[counter+1],150-(50*i));
        glEnd();
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glColor3f(1.0,1.0,1.0);
        glVertex2f(100+gvars::array[counter],200-(50*i));
        glVertex2f(100 + gvars::array[counter+1],150-(50*i));
        //glEnd();
        counter++;
        glEnd();
    }
    glPopMatrix();
    glLineWidth(1);
}

void lighting( int size, int start, int end){
    for(int i=0; i<size;i++) {
        int randsign = rand()%2;
        int randXnum = randsign ? rand()%((end-start)+start): -(rand()%((end-start)+start));
        gvars::array[i]=randXnum;
    }
}

