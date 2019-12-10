/*Author Gerardo Martinez-Flores
 *Date: 10 December 2019
 */
/*Four steps of Debugging
 *1)Recognize that a bug exists
 *2)Isolate the source of the bug
 *3)Identify the cause of the bug
 * 4)Determine a fix for the bug
 */
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
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
#include <fstream>
#include "Texture.h"
#include <sys/utsname.h>
#include <algorithm>

#define DEG2RAD 3.14159/180.0
//void drawlight(int x1, int y1,int x2,int y2,int displace);
void drawSquare (int width, int height, int dirX, int dirY);
void boxText(Rect r);
extern void postScores(char*);
using namespace gvars;
void creditsGerardo(Rect r)
{
    ggprint8b(&r, 16, 0x00004C00, "Gerardo Martinez Flores");
}
//this is the makebutton that dynamically changes when the window change size
void makeButton(int x, int y, int dirX, int dirY)
{
    int width = x/16;
    int height = y/26;
    int posx[4] = {x/5,0, -(x/5),0};
    int posy[4] = {(-y/3)-y/15, (-y/3)-y/15, (-y/3)-y/15,-(y/4)-(y/70)};
    glPushMatrix();

    glBegin(GL_QUADS);
    glColor3f(0.5,0.0,0.0);
    for(int i=0;i<3;i++) {
        dirX = posx[i];
        dirY = posy[i];
        drawSquare(width, height, dirX, dirY);
    }
    dirX = posx[3];
    dirY = posy[3];
    drawSquare(x/4 + x/70, height, dirX, dirY);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.7,0.13,0.13);
    for(int i=0;i<4;i++) {
        dirX = posx[i];
        dirY = posy[i];
        drawSquare(width - 3, height - 3, dirX, dirY);
    }
    dirX = posx[3];
    dirY = posy[3];
    drawSquare(x/4 + x/70 - 3, height-3, dirX, dirY);
    glEnd();

    glPopMatrix();
}

void changeButtonColor( int x, int y ,int dirX, int dirY, int choice) 
{

    int width = x/16;
    int height = y/26;
    int posx[4] = {x/5,0, -(x/5),0};
    int posy[4] = {(-y/3)-y/15, (-y/3)-y/15, (-y/3)-y/15,-(y/4)-(y/70)};
    /*
       int posx[4] = {x/5,0, -(x/5)};
       int posy[4] = {(-y/4), (-y/4), (-y/4)};*/
    if( choice == 2) {
        dirX=posx[3];
        dirY=posy[3];
        glColor3f(0.0,1.0,1.0);
        drawSquare(x/4+x/70 - 3, height - 3,dirX,dirY);
    }

    if( choice == 1) {
        dirX=posx[0];
        dirY=posy[0];
        glColor3f(0.0,1.0,1.0);
        drawSquare(width - 3, height - 3,dirX,dirY);
    }
    //the boxes are backwards in the array
    if( choice == 3) {
        dirX=posx[2];
        dirY=posy[2];
        glColor3f(0.0,1.0,1.0);
        drawSquare(width -3, height - 3,dirX,dirY);
    }

    if( choice == 4) {
        dirX=posx[1];
        dirY=posy[1];
        glColor3f(0.0,1.0,1.0);
        drawSquare(width - 3, height - 3,dirX,dirY);
    }
}

Texture character1;
Texture character2;
Texture character3;

void characterOption(int option)
{

    if(option == 1) {
        character1.set("./images/trooper_sprite/trooper_s.png");
        character1.Display_Picture( 50, 50, -200, 0);
    } else if (option == 2) {
        character2.set("./images/ninja_s.png");
        character2.Display_Picture( 50, 50, 0, 0);
    } else if(option ==3) {
        character3.set("./images/hitler_front.png");
        character3.Display_Picture( 50, 50, 200, 0);
    }
}

Texture character4;
Texture character5;
Texture character6;
//This is what the character will see when pressing character button
void characterChoice() 
{

    character4.set("./images/trooper_sDark.png");
    character4.Display_Picture( 50, 50, -200, 0);
    character5.set("./images/hitler_frontDark.png");
    character5.Display_Picture( 50, 50, 200, 0);
    character6.set("./images/ninja_sDark.png");
    character6.Display_Picture( 50, 50, 0, 0);
}
//Display current score after game over
void displaycurrentscore(Rect r, int h, int w, int bestScore,int yourScore)
{
    r.bot=h-h;
    r.left=w-w-25;
    r.center=0;
    ggprint16(&r, 16, 0x003B8B68, "Game Over\n");
    ggprint16(&r, 16, 0x003B8B68, "Your score:%d \n", yourScore);
    ggprint16(&r, 16, 0x003B8B68, "Best score:%d \n", bestScore);
    ggprint16(&r, 16, 0x003B8B68, "\nTap to restart\n");
    char pn [1000];

    sprintf(pn, "~mbal/3350/lab7/scores.php?name=%s&score=%d",getenv("USER"),yourScore); 
    
    postScores(pn);
}
//Text for the box for makeButton
void boxText(Rect r, int x, int y) 
{
    std::string boxText[4]={"Characters(t)","How To Play(j)","Credits(c)","Start Game(space)"};
    //determine the text positioin for each button in array.
    int posx[4] = {x/5-x/35,0-35,-(x/4)+x/30,0-35};

    int posy[4] = {(-y/3)-y/13, (-y/3)-y/13, (-y/3)-y/13,-(y/4)-y/50};
    /*
       int posx[3]={-x/4+x/40, -x/50, x/6+x/40};
       int posy[3]={-y/4-y/95,-y/4-y/95,-y/4-y/95};
       */
    for(int i=0;i<4;i++) {
        r.left=posx[i];
        r.bot=posy[i];
        r.center=0;
        ggprint8b(&r,16,0x00cefdce, boxText[i].c_str());
    }

}
//This is the text string for the character select button 
//to have characters names under the image
void characterText(Rect r, int x, int y) 
{
    std::string boxText[3]={"Hilter","Hitler Cousin","Hitler BFF"};

    int posx[3]={-x/4+x/40, -x/50, x/6+x/40};
    int posy[3]={-y/4-y/95,-y/4-y/95,-y/4-y/95};
    for(int i=0;i<3;i++) {
        r.left=posx[i];
        r.bot=posy[i];
        r.center=0;
        ggprint8b(&r,16,0x00cefdce, boxText[i].c_str());
    }

}
//To draw the square box for makeButton
void drawSquare(int x, int y, int dirX, int dirY) 
{
    int w=x;
    int h=y;
    glBegin(GL_QUADS);
    glVertex2i(w+dirX, -h+dirY);
    glPushMatrix();
    //draw the box four points
    glVertex2i(w+dirX, h+dirY);
    glVertex2i(-w+dirX, h+dirY);
    glVertex2i(-w+dirX, -h+dirY);
    glVertex2i(w+dirX, h+dirY);
    glVertex2i( w+dirX, h+dirY);
    glEnd();
    glPopMatrix();
}
void populateWithRand(int * array, unsigned int size, int start, int end)
{
    //randomly store a valu to create a realistic ligthing 
    //every time it is rendered
    for(unsigned int i = 0; i < size;i++){
        int randsign = rand()%2;
        int randXnum = randsign ? rand()%(end-start)+start: 
            -(rand()%(end-start)+start);
        array[i] = randXnum;
    }
}
void drawLine() 
{
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
//used to create the lighting bolts that randomly generate diffferent bolts.
void lighting( int size, int start, int end)
{
    for(int i=0; i<size;i++) {
        int randsign = rand()%2;
        int randXnum = randsign ? rand()%((end-start)+start): -(rand()%((end-start)+start));
        gvars::array[i]=randXnum;
    }
}
//this is a test to see if the scoreboard is working
void scoreboard(Rect r) 
{
    //    glClear(GL_COLOR_BUFFER_BIT);
    r.bot =(gl.yres/2)-20;
    r.left=10;
    r.center=0;
    ggprint8b(&r, 16, 0x00ff0000,"This is working?");
}

void drawVine() 
{
    glPushMatrix();
    int counter=0;
    for(int i=0;i<10;i++) {
        glLineWidth(7);
        glBegin(GL_LINE_LOOP);
        glColor3f(0.0,0.3,0.2);
        glVertex2f(100+gvars::arrayVine[counter],200-(50*i));
        glVertex2f(100+gvars::arrayVine[counter+1],150-(50*i));
        glEnd();
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glColor3f(0.0,0.6,0.37);
        glVertex2f(100+gvars::arrayVine[counter],200-(50*i));
        glVertex2f(100 + gvars::arrayVine[counter+1],150-(50*i));
        //glEnd();
        counter++;
        glEnd();
    }
    glPopMatrix();
    glLineWidth(1);
}
//This is my Friday code.
void renderVine( int size, int start, int end)
{
    for(int i=0; i<size;i++) {
        int randsign = rand()%2;
        int randXnum = randsign ? rand()%((end-start)+start): -(rand()%((end-start)+start));
        gvars::arrayVine[i]=randXnum;
    }    
}
//This where the text will display in How To Play
void DisplayGameControls(Rect r)
{
    r.center=0;
    r.bot=220;
    r.left=-490;
    ggprint16(&r, 16, 0x003B8B68, "How To Play\n");
    ggprint16(&r, 16, 0x003B8B68, "To move the character\n");
    ggprint16(&r, 16, 0x003B8B68, "    -Up arrow moves the playe north\n");
    ggprint16(&r, 16, 0x003B8B68, "    -Down arrow moves the player south\n");
    ggprint16(&r, 16, 0x003B8B68, "    -Left arrow moves the player east\n");
    ggprint16(&r, 16, 0x003B8B68, "    -Rigt arrow moves the player west\n");
    ggprint16(&r, 16, 0x003B8B68, "    -Up + Left arrows moves the player North East\n");
    ggprint16(&r, 16, 0x003B8B68, "    -Up + Right arrows moves the player North West\n");
    ggprint16(&r, 16, 0x003B8B68, "    -Down + Left arrows moves the player South East\n");
    ggprint16(&r, 16, 0x003B8B68, "    -Down + Right arrows moves the player South West\n");
    ggprint16(&r, 16, 0x003B8B68, "To Shoot\n");
    ggprint16(&r, 16, 0x003B8B68, "    -Right click on the mouse\n");
    ggprint16(&r, 16, 0x003B8B68, "Change Bullet Elements\n");
    ggprint16(&r, 16, 0x003B8B68, "    -Press e to change the element to either Fire, Water, Earth, and Air\n");
}
