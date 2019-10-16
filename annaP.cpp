/*
 * Source File
 * By: Anna Poon
 * Source File 
 * Main Purpose: Start Menu, Animations, Power-Ups, Enemy AI
*/

#include <iostream>
#include <cmath>
#include <GL/glx.h>
#include "fonts.h"
#include <fstream>

/*Summary of Source File
	Start Menu Function is used to display the start menu at the start of game.
	Display Image is a function used to display any image in game at any location of the screen
	Display Background is a function to display the background in fullscreen.
*/

/*Debugging Four Steps
 *
 * Isolate
 * Identify
 * Determine a fix
 * Apply and Test
*/


typedef float Vec[3];
int scores[5];
std::string names[5];
char pagename[256] = "~mbal/3350/lab7/scores.txt";
const float DEG2RAD = 3.14159 / 180;

/*Prototype Functions for Functions Used*/
void movingEyes(int *eye, int *location);
void displayBackground(int w, int h, unsigned int texture);
void displayImage(int width_x, int height_y, int offset_x, int offset_y, unsigned int texture);
extern void displaycurrentscore(Rect r, int h, int w, int bestScore, int yourScore);
extern void readScores(char * filename);

/*Function Definitions*/
void startMenu(Rect r, int y, int x, int img_x, int img_y, unsigned int startMenuTexture)
{
	int eyeLeft[2] = {15, 15};
	int eyeRight[2] = {-15, 15};
	int leftLocation[2] = {32, 50};
	int rightLocation[2] = {-47, 50};
	img_y = 0 + img_x;
	displayImage(img_y / 2, img_y / 2, 0, 50, startMenuTexture);
	movingEyes(eyeLeft, leftLocation);
	movingEyes(eyeRight, rightLocation);
	r.bot = 0 - (y / 3);
	r.left = 0 - (x / 7);
	r.center = 0;
	ggprint16(&r, 16, 0x00ff0000, "Press Space to Continue");
	ggprint16(&r, 16, 0x00ff0000,
			  "Press Space + C for Credits During Gameplay");
	ggprint16(&r, 16, 0x00ff0000,
			  "Press Space + G for GameOver Screen During Gameplay");
	ggprint16(&r, 16, 0x00ff0000,
			  "Press Space + H for Highscores Screen During Gameplay");
}
void highScoreBoard(Rect r2, int w, int h, unsigned int imageTexture)
{
	glClear(GL_COLOR_BUFFER_BIT);
	displayBackground(w, h, imageTexture);
	r2.bot = h / 2 - 25;
	r2.left = w / 2 - 225;
	r2.center = 0;
	ggprint8b(&r2, 16, 0x00ff0000,
			  "Press H to Toggle back to Gameplay");
	r2.bot = h / 2 - 100;
	r2.left = 0 - 25;
	r2.center = 0;
	ggprint16(&r2, 100, 0x00ff0000, "HighScores");
	r2.left = -300;
	ggprint16(&r2, 0, 0x00ff0000,"USERNAMES");
	r2.left = 300;
	ggprint16(&r2, 100, 0x00ff0000, "Scores");

	for (int i = 0; i < 5; i++) {
		r2.left = -300;
		ggprint16(&r2, 0, 0x00ff0000, names[i].c_str());
		r2.left = 300;
		char buf[20];
		sprintf(buf, "%d", scores[i]);
		ggprint16(&r2, 50, 0x00ff0000, buf);
	}
}
void displayGameOverScore(Rect r2, int w, int h, unsigned int imageTexture, int currentScore)
{
	glClear(GL_COLOR_BUFFER_BIT);
	displayBackground(w, h, imageTexture);
	r2.bot = h / 2 - 25;
	r2.left = -w / 2 + 25;
	r2.center = 0;
	ggprint16(&r2, 16, 0x00ff0000,
			  "Press H for HighScores Screen");
	displaycurrentscore(r2, h, w, scores[0], currentScore);
}
void getScores(char *filename, int &grabHighScores)
{
	std::ifstream highscore;
	readScores(pagename);
	try {
		highscore.open(filename);
	}
	catch (...) {
		std::cout << "An exception has occurred" << std::endl;
		exit(1);
	}
	//first we need to ignore the extra info in the file
	std::string temp;
	while (getline(highscore, temp)) {
		if (temp == "Start") {
			for (int i = 0; i < 5; i++) {
				highscore >> names[i] >> scores[i];
			}
		}
	}
	grabHighScores = 1;
}
void displayImage(int width_x, int height_y, int offset_x, int offset_y, unsigned int texture)
{
	int width = width_x / 2;
	int height = height_y / 2;
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);

	glTexCoord2f(1, 1);
	glVertex2i(width + offset_x, -height + offset_y);
	glTexCoord2f(1, 0);
	glVertex2i(width + offset_x, height + offset_y);
	glTexCoord2f(0, 0);
	glVertex2i(-width + offset_x, height + offset_y);
	glTexCoord2f(0, 1);
	glVertex2i(-width + offset_x, -height + offset_y);

	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
}
void movingImages(int width_x, int height_y, Vec img_pos, float img_angle, unsigned int texture)
{
	int width = width_x / 2;
	int height = height_y / 2;
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);

	glTranslatef(img_pos[0], img_pos[1], img_pos[2]);
	glTranslatef(0, 0, 0);
	glRotatef(img_angle, 0.0f, 0.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);

	glTexCoord2f(1, 1);
	glVertex2i(width, -height);
	glTexCoord2f(1, 0);
	glVertex2i(width, height);
	glTexCoord2f(0, 0);
	glVertex2i(-width, height);
	glTexCoord2f(0, 1);
	glVertex2i(-width, -height);

	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
}
void displayBackground(int w, int h, unsigned int texture)
{
	int width = w / 2;
	int height = h / 2;
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex2i(width, -height);
	glTexCoord2f(1, 0);
	glVertex2i(width, height);
	glTexCoord2f(0, 0);
	glVertex2i(-width, height);
	glTexCoord2f(0, 1);
	glVertex2i(-width, -height);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
}
void movingEyes(int *eye, int *location)
{
	int width = eye[0]; //, height = eye[1];
	int offset_x = location[0], offset_y = location[1];
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 1);
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < 360; i++) {
		float degInRad = i * DEG2RAD;
		glVertex2f(cos(degInRad) * (width / 2) + offset_x, sin(degInRad) * (width / 2) + offset_y);
	}

	glEnd();
	glPopMatrix();
}
void creditsAnna(Rect r)
{
	ggprint8b(&r, 16, 0x00004C00, "Anna Poon");
}
