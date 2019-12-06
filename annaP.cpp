/*
 * Source File
 * By: Anna Poon
 * Source File 
 * Main Purpose: Start Menu, Animations, 
 * Power-Ups, Enemy AI
*/

#include <iostream>
#include <cmath>
#include <GL/glx.h>
#include "fonts.h"
#include <fstream>
#include <unistd.h>
#include "GlobalSpace.h"
#include "Texture.h"
#include "Image.h"

/*Summary of Source File
	Start Menu Function is used to display the 
	start menu at the start of game.
	Display Image is a function used to display 
	any image in game at any location of the screen
	Display Background is a function to display 
	the background in fullscreen.
*/

/*Debugging Four Steps
 *
 * Isolate
 * Identify
 * Determine a fix
 * Apply and Test
*/
using namespace gvars;

typedef float Vec[3];
int scores[5];
std::string names[5];
char pagename[256] = "~mbal/3350/lab7/scores.txt";
const float DEG2RAD = 3.14159 / 180;
int showTitle = 1000000;

/*Prototype Functions for Functions Used*/
void movingEyes(int *eye, int *location);
void fireCircles(int, int, int);
void waterBubbles(int offset_x, int offset_y);
void lightningShots(float angle, int offset_x, int offset_y);
void grassRazorLeaf(float angle, int offset_x, int offset_y);
void grassRazorMove(int & x);
void waterBubbleMove(int & y);
void displayBackground(int w, int h, unsigned int texture);
void displayElementSelection(unsigned int * imageTexture, int choice);
void displayImage(int width_x, int height_y, int offset_x,
int offset_y, unsigned int texture);
extern void displaycurrentscore(Rect r, int h, int w, int bestScore, int yourScore);
extern void readScores(char * filename);

Texture::Texture()
{
}

Texture::Texture(const char*fname, int x1, int y1, int z1, int w1, int h1):w(w1), h(h1), x(x1), y(y1), z(z1){
	img = new Image(fname);
	w = img->width;
	h = img->height;
	unsigned char * tpimage = buildAlphaData();
    glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, tpimage);
}
Texture::~Texture()
{
	delete img;
}
void Texture::set(const char*fname)
{
	img = new Image(fname);
	w = img->width;
	h = img->height;
	unsigned char * tpimage = buildAlphaData();
    glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, tpimage);

}
void Texture::Display_Picture(int xres, int yres, int offx, int offy){
    int width = xres;
	int height = yres;
    glPushMatrix();
    //glColor3f(1.0,1.0,1.0);
	//glRotatef(318, 0.0, 1.0, 0.0);
	//glTranslatef(0, 100, 0);
    glBindTexture(GL_TEXTURE_2D, id);
    glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3i(-width+offx,height+offy, 0); 
        glTexCoord2f(0, 1);
        glVertex3i(-width+offx,-height+offy, 0); 
        glTexCoord2f(1, 1);
        glVertex3i(width+offx, -height+offy, 0);      
        glTexCoord2f(1,0);
        glVertex3i(width+offx,height+offy, 0);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}
int Texture::getID()
{
	return id;
}
unsigned char* Texture::buildAlphaData()
{
	//add 4th component to RGB stream...
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a,b,c;
	//use the first pixel in the image as the transparent color.
	unsigned char t0 = *(data+0);
	unsigned char t1 = *(data+1);
	unsigned char t2 = *(data+2);
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a==t0 && b==t1 && c==t2)
			*(ptr+3) = 0;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	
	return newdata;
}


/*Function Definitions*/
void startMenu(Rect r, int y, int x, int img_x, int img_y, unsigned int startMenu, unsigned int title)
{
	int eyeLeft[2] = {img_x / 100, img_y / 50};
	int eyeRight[2] = {-img_x / 100, img_y / 50};
	int leftLocation[2] = {img_x - (img_x - img_x / 23), 
		img_y - (img_y - img_y / 25)};
	int rightLocation[2] = {(img_x - img_x / 25) - img_x, 
		img_y - (img_y - img_y / 25)};
	
	displayImage(img_x / 8, img_y / 4 + img_y / 20, 0, img_y / 40, startMenu);
	displayImage(img_x / 4, img_y / 10, 0, img_y /3 + img_y / 12, title);

	movingEyes(eyeLeft, leftLocation);
	movingEyes(eyeRight, rightLocation);

	r.bot = 0 - (y / 3);
	r.left = 0 - (x / 9);
	r.center = 0;

	ggprint8b(& r, 16, 0x00ff0000, "Press Space to Continue");
	ggprint8b(& r, 16, 0x00ff0000,
		"Press M for Menu Screen During Gameplay");
	ggprint8b(& r, 16, 0x00ff0000,
		"Press C for Credits During Gameplay");
	ggprint8b(& r, 16, 0x00ff0000,
		"Press G for GameOver Screen During Gameplay");
	ggprint8b(& r, 16, 0x00ff0000,
		"Press H for Highscores Screen During Gameplay");
}
void highScoreBoard(Rect r2, int w, int h, unsigned int imageTexture)
{
	glClear(GL_COLOR_BUFFER_BIT);
	displayBackground(w, h, imageTexture);
	r2.bot = h / 2 - 25;
	r2.left = w / 2 - 225;
	r2.center = 0;
	ggprint8b(& r2, 16, 0x00ff0000,
		"Press M to Toggle back to Menu");
	r2.bot = h / 2 - 100;
	r2.left = 0 - 25;
	r2.center = 0;
	ggprint16(& r2, 100, 0x00ff0000, "HighScores");
	r2.left = -300;
	ggprint16(& r2, 0, 0x00ff0000,"USERNAMES");
	r2.left = 300;
	ggprint16(& r2, 100, 0x00ff0000, "Scores");

	for (int i = 0; i < 5; i++) {
		r2.left = -300;
		ggprint16(& r2, 0, 0x00ff0000, names[i].c_str());
		r2.left = 300;
		char buf[20];
		sprintf(buf, "%d", scores[i]);
		ggprint16(& r2, 50, 0x00ff0000, buf);
	}
}
void displayGameOverScore(Rect r2, int w, int h, unsigned int imageTexture, int currentScore)
{
	glClear(GL_COLOR_BUFFER_BIT);
	displayBackground(w, h, imageTexture);
	r2.bot = h / 2 - 25;
	r2.left = -w / 2 + 25;
	r2.center = 0;
	ggprint16(& r2, 16, 0x00ff0000, "Press M to go back to Menu");
	displaycurrentscore(r2, h, w, scores[0], currentScore);
}
void getScores(char *filename)
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
}
void displayImage(int width_x, int height_y, int offset_x, int offset_y, unsigned int texture)
{
	int width = width_x;
	int height = height_y;
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
void movingImages(int width_x, int height_y, Vec img_pos, 
float img_angle, unsigned int texture)
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
void spinningIntro(int width_x, int height_y, int offset_x, int offset_y, 		unsigned int texture)
{
	glTranslatef(10, 0, 0);
	//glRotatef(0.0, 0.45f, 0.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);

	glTexCoord2f(1, 1);
	glVertex2i(width_x + offset_x, -height_y + offset_y);
	glTexCoord2f(1, 0);
	glVertex2i(width_x + offset_x, height_y + offset_y);
	glTexCoord2f(0, 0);
	glVertex2i(-width_x + offset_x, height_y + offset_y);
	glTexCoord2f(0, 1);
	glVertex2i(-width_x + offset_x, -height_y + offset_y);

	glEnd();
	glPopMatrix();
}
void runLogoIntro(unsigned int logoIntroTexture)
{
	Rect r; 
	spinningIntro(150, 150, 20, 0, logoIntroTexture);
	r.bot = -215;
	r.left = -95;
	r.center = 0;
	ggprint16(& r, 0, 0x00c0c0c0, "Undead Games Presents"); 
	std::fflush(stdout); 

}
void movingEyes(int * eye, int * location)
{
	int width = eye[0] * 2;
	int offset_x = location[0], offset_y = location[1];
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 1);
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < 360; i++) {
		float degInRad = i * DEG2RAD;
		glVertex2f(cos(degInRad) * (width / 2) + offset_x, sin(degInRad) * (width / 2) + offset_y);
	}
	glEnd();
	glPopMatrix();
}
void enemyAI(Vec trooper_pos, float trooper_angle, Vec enemy_pos, float enemy_angle, int xres, int yres)
{
	enemy_pos[1] = (int)((enemy_pos[1] + (trooper_pos[1]*0.012))) % yres;
	enemy_pos[0] = (int)((enemy_pos[0] + (trooper_pos[0]*0.012))) % xres;
	enemy_angle = trooper_angle*0.5 + enemy_angle;
	/* if ((enemy_pos[1] > (trooper_pos[1] + 5)) 
	&& (enemy_pos[0] > (trooper_pos[0] + 5)))*/
}
/*Anna 
	-function needs to know where to draw the circle
	-where to move circle
	-size of circle
	-draw a circle
*/

/* Elemental Bullets*/
void fireCircles(int row, int offset_x, int offset_y)
{
	int x = 300, y = 300, w = 5, h = 5;
	
	if (offset_x && offset_y) {
		x = offset_x;
		y = offset_y;
	}

	glColor3ub(gvars::fireColors[row][0], gvars::fireColors[row][1],gvars::fireColors[row][2]);
	glPushMatrix();
	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i < 360; i+=40) {
		float degInRad = i * DEG2RAD;
		glVertex2f(cos(degInRad) * (w) + x, sin(degInRad) * (h) + y);
	}
	glEnd();
	
}

/* Anna, you need to fix where the lightning's position is*/

void lightningShots(float angle, int offset_x, int offset_y){
	
	float x_angle = cos(angle);
	float y_angle = sin(angle);

	glPushMatrix();

	glLineWidth(7);
	glBegin(GL_LINES);
	glColor3ub(gvars::lightningColors[0][0], gvars::lightningColors[0][1],gvars::lightningColors[0][2]);
	glVertex2f(x_angle*offset_x, y_angle*offset_y);
    glVertex2f(x_angle*(20  + offset_x), y_angle*(20 + offset_y));
	glEnd();

	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3ub(gvars::lightningColors[1][0], gvars::lightningColors[1][1],gvars::lightningColors[1][2]);
	glVertex2f(x_angle*offset_x, y_angle*offset_y);
    glVertex2f(x_angle*(20  + offset_x), y_angle*(20 + offset_y));
	
	glEnd();
	
}

void grassRazorLeaf(float angle, int offset_x, int offset_y){
	
	int width = 5, height = 5;

	glPushMatrix();

	glLineWidth(4);

	glBegin(GL_POLYGON);

	glColor3ub(gvars::grassColors[0][0], gvars::grassColors[0][1],gvars::grassColors[0][2]);
	
	glVertex2i(-width + offset_x, height + offset_y);
	glVertex2i(width + offset_x, -height + offset_y);
	glVertex2i(width + offset_x, height + offset_y);
	glVertex2i(-width + offset_x, -height + offset_y);
	
	glEnd();
	
	glBegin(GL_POLYGON);

	glColor3ub(gvars::grassColors[2][0], gvars::grassColors[2][1],gvars::grassColors[2][2]);
	
	glVertex2i(-width + offset_x, height + offset_y);
	glVertex2i(width + offset_x, -height + offset_y);
	glVertex2i(width + offset_x, height + offset_y);
	glVertex2i(-width + offset_x, -height + offset_y);
	
	glEnd();
	angle = offset_x + angle;
}

void grassVines(float, int, int){
	;
}
void waterBubbles(int offset_x, int offset_y)
{
	int x = 300, y = 300, w = 7, h = 7;
	if (offset_x && offset_y) {
		x = offset_x;
		y = offset_y;
	}

	glColor3ub(gvars::waterColors[1][0], gvars::waterColors[1][1],gvars::waterColors[1][2]);
	glPushMatrix();
	glBegin(GL_TRIANGLE_FAN);

	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 360; i+=40) {
			float degInRad = i * DEG2RAD;
			glVertex2f(cos(degInRad) * (w) + (x), sin(degInRad) * (h) + (y));
		}
	}
	w = 3; 
	h = 3;

	glColor3ub(gvars::waterColors[2][0], gvars::waterColors[2][1],gvars::waterColors[2][2]);
	glPushMatrix();
	glBegin(GL_TRIANGLE_FAN);

	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 360; i+=40) {
			float degInRad = i * DEG2RAD;
			glVertex2f(cos(degInRad) * (w) + (x), sin(degInRad) * (h) + (y));
		}
	}
	glEnd();
	
}

/*Functions for Different Bullet Movements*/

void grassRazorMove(int & x)
{
	x *= 20;
}
void waterBubbleMove(int & y)
{
	y  += .15;
}
void fireWaveMove(int & x, int & y)
{
	y  += .15;
	x  *= 0.7;
}

/* Function to Decide Which Type of Element to use */
void displayElementSelection(unsigned int * imageTexture, int choice){
	switch(choice){
		case 0:
			displayImage(100, 100, 0 , 0, imageTexture[choice]);
			break;
		case 1:
			displayImage(100, 100, 0 , 0, imageTexture[choice]);
			break;
		case 2: 
			displayImage(100, 100, 0 , 0, imageTexture[choice]);
			break;
		case 3:
			displayImage(100, 100, 0 , 0, imageTexture[choice]);
			break;
	}
}

void creditsAnna(Rect r)
{
	ggprint8b(& r, 16, 0x00004C00, "Anna Poon");  
}

void writing(Rect r, std::string sentence)
{
	ggprint8b(& r, 16, 0x00004C00, sentence.c_str());  
}
// This is my Friday code. 
Texture water("images/water.png", 0,0,0, gl.xres, gl.yres);
Texture grass("images/leaf.png", 0,0,0, gl.xres, gl.yres);
Texture light("images/electric.png", 0,0,0, gl.xres, gl.yres);
Texture fire("images/fire.png", 0,0,0, gl.xres, gl.yres);

void switchBullets(float angle, int row, int offset_x, int offset_y, int choice){
	switch(choice){
		case 0:
			fireCircles(row, offset_x, offset_y);
			fireWaveMove(offset_x, offset_y);
			break;
		case 1:
			waterBubbles(offset_x, offset_y);
			waterBubbleMove(offset_y);
			//gl.yres / 2 - 200);
			break;
		case 2: 
			grassRazorLeaf(angle, offset_x, offset_y);
			grassRazorMove(offset_x);
			break;
		case 3:
			lightningShots(angle, offset_x, offset_y);

			break;
	}
}
void showAttack(int choice) 
{
	switch(choice){
		case 0:
			fire.Display_Picture(70, 50, gl.xres / 2 - 70, -(gl.yres / 2 - 100));
			break;
		case 1:
			water.Display_Picture(30, 50, gl.xres / 2 - 70, -(gl.yres / 2 - 100));
			break;
		case 2: 
			grass.Display_Picture(30, 50, gl.xres / 2 - 70, -(gl.yres / 2 - 100));
			break;
		case 3:
			light.Display_Picture(30, 35, gl.xres / 2 - 60, -(gl.yres / 2 - 100));
			break;
	}
}



