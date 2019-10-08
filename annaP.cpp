/*
 * Source File
 * By: Anna Poon
 * Source File 
 * Main Purpose: Start Menu, Animations, Enemy AI
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
#include <cstring>
#include <fstream>

/*Summary of Source File
	Start Menu Function is used to display the start menu at the start of game.
	Display Image is a function used to display any image in game at any location of the screen
	Display Background is a function to display the background in fullscreen.
*/

typedef float Vec[3];
std::string scores[100];
std::string names[100];
char pagename[256] = "~gmartinezflo/3350/lab7/test.txt";


/*Prototype Functions*/
void startMenu(Rect r, int y, int x, int img_x, int img_y, GLuint imageTexture);
void highScoreBoard(Rect r, int w, int h, GLuint imageTexture);
void displayImage(int width_x, int height_y, int offset_x, int offset_y, GLuint texture);
void displayBackground(int w, int h, GLuint texture);
void creditsAnna(Rect r);
extern void readScores(char*);
void displayGameOverScore(Rect r2, int w, int h, GLuint imageTexture, int currentHighScore, int currentScore, int &gameOver);
extern void displaycurrentscore(Rect r, int h, int w, int bestScore,int yourScore);
/*Function Definitions*/
void startMenu(Rect r, int y, int x, int img_x, int img_y, GLuint startMenuTexture)
{

	/*img_x = img_y + startMenuTexture;*/
	img_y = 0 + img_x;
	displayImage(img_y/2,img_y/2, 0, 50, startMenuTexture);
    	r.bot = 0-(y/3);
    	r.left = 0-(x/7);
    	r.center = 0;
    ggprint16(&r, 16, 0x00ff0000, "Press Space to Continue");
    ggprint16(&r, 16, 0x00ff0000, 
            "Press Space + C for Credits During Gameplay");
}
void highScoreBoard(Rect r2, int w, int h, GLuint imageTexture){
    glClear(GL_COLOR_BUFFER_BIT);
    displayBackground(w, h, imageTexture);
    r2.bot = h/2-25;
    r2.left = -w/2+25;
    r2.center = 0;
    ggprint16(&r2, 16, 0x00ff0000, 
        "Press H to Toggle back to Gameplay");
    r2.bot = h/2-100;
    r2.left = 0-25;
    r2.center = 0;
    ggprint16(&r2, 16, 0x00ff0000, 
        "HighScores");

    for(int i = 14; i < 20; i++){
        r2.left = -100;
        r2.bot -= 50;
        ggprint16(&r2, 16, 0x00ff0000,  names[i].c_str());
        r2.left = 100;
        ggprint16(&r2, 16, 0x00ff0000,  scores[i].c_str());
    }
} 

void displayGameOverScore(Rect r2, int w, int h, GLuint imageTexture, int currentHighScore, int currentScore){
    glClear(GL_COLOR_BUFFER_BIT);
    displayBackground(w, h, imageTexture);
    r2.bot = h/2-25;
    r2.left = -w/2+25;
    r2.center = 0;
    ggprint16(&r2, 16, 0x00ff0000, 
        "Press H for HighScores Screen");

    displaycurrentscore(r2, h, w, currentHighScore, currentScore);
    /*
    r2.bot = h/2-100;
    r2.left = 0-25;
    r2.center = 0;
    ggprint16(&r2, 16, 0x003B8B68, 
        "Game Over");
    ggprint16(&r2, 16, 0x003B8B68, 
        "Your Score:%d", currentScore);
    ggprint16(&r2, 16, 0x003B8B68, 
        "Best Score:%s", currentHighScore);
    ggprint16(&r2, 16, 0x00ff0000, 
        "Press C to go back to game");*/
    
} 

void getScores(char*filename, int &grabHighScores){
    std::ifstream highscore;
    readScores(pagename);
    try{
        highscore.open(filename);
    }
    catch(...){
        std::cout << "An exception has occurred" << std::endl;
        exit(1);
    }
    int i = 0;
    while(highscore >> names[i] >> scores[i]){
        i++;
    }
    grabHighScores = 1;
}
void displayImage(int width_x, int height_y, int offset_x, int offset_y, GLuint texture)
{
	int width = width_x/2;
	int height = height_y/2;
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
	
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
       
        glTexCoord2f(1, 1);
        glVertex2i(width+offset_x, -height+offset_y);
        glTexCoord2f(1,0);
        glVertex2i(width+offset_x,height+offset_y);
		glTexCoord2f(0, 0);
        glVertex2i(-width+offset_x,height+offset_y);
        glTexCoord2f(0, 1);
        glVertex2i(-width+offset_x,-height+offset_y);

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

void movingImages(int width_x, int height_y, Vec img_pos, float img_angle, GLuint texture)
{
	int width = width_x/2;
	int height = height_y/2;
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
	/*Adding Code for the Trooper*/
	glTranslatef(img_pos[0], img_pos[1], img_pos[2]);
	glTranslatef(0, 0, 0);
	glRotatef(img_angle, 0.0f, 0.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
       
        glTexCoord2f(1, 1);
        glVertex2i(width, -height);
        glTexCoord2f(1,0);
        glVertex2i(width, height);
		glTexCoord2f(0, 0);
        glVertex2i(-width,height);
        glTexCoord2f(0, 1);
        glVertex2i(-width,-height);

    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

void displayBackground(int w, int h, GLuint texture)
{
	int width = w/2;
	int height = h/2;
    	glPushMatrix();
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
