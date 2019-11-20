//
//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014 - 2018
//mod spring 2015: added constructors
//This program is a game starting point for a 3350 project.
//
//
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
#include <fstream>
#include <cstring>

//Include Files for Classes
#include "Image.h"
#include "Global.h"
#include "Trooper.h"
#include "Skull.h"
#include "Bullet.h"
#include "Asteroid.h"
#include "Game.h"
#include "X11.h"
#include "Zombie.h"
#include "Texture.h"
#include "BlenderObj.h"




unsigned int bloodBackgroundTexture; 
//image for zombie
// unsigned int startMenuTexture; 
unsigned int logoIntroTexture; 

//File for Reading In HighScore
char filename[] = "scores.txt";

//These are Flags for the game. 
extern struct timespec timeStart, timeCurrent;
extern struct timespec timePause;
extern double physicsCountdown;
extern double timeSpan;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------
// Set Up Images

Image img[9] = {
		"./images/background.png",
		"./images/zombie_start.png",
		"./images/trooper.png",
		"./images/ghost_skull.png",
		"./images/undead_logo.png",
		"./images/bloodBackground.png",
		"./images/title.png",
		"./images/zombie.png", 
		"./images/water.png"
};


class Position {
public:
	Vec pos;
	Vec vel;
} position;


//function prototypes
void init_opengl(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();
void renderCredits();

extern void creditManvir(Rect r);
extern void creditsAnna(Rect r);
extern void creditsGerardo(Rect r);
extern void creditsKevin(Rect r);
extern void startMenu(Rect r, int y_num, int x_num, int img_x, int img_y, unsigned int startMenuTexture, unsigned int titleImageTexture);
extern void movingImages(int width_x, int height_y, Vec img_pos, float img_angle, unsigned int texture);
extern void randomColor();
extern void renderCoolCredits(int w, int h, unsigned int imageTexture);
extern void makeParticles(int, int);
extern void getScores(char*);
extern void makeButton(int x, int y, int dirX, int dirY);
extern void drawLine();
extern void scoreboard(Rect r);
extern void lightningShoots(float, int, int);
extern void fireCircles(int, int, int);
extern void grassVines(float, int, int);
extern void waterBubbles(int, int);
extern void boxText(Rect r, int, int );
extern void lighting( int size, int start, int end);
extern void runLogoIntro(unsigned int logoIntroTexture);
extern void changeButtonColor( int , int ,int dirX, int dirY, int choice);
extern void highScoreBoard(Rect, int, int, unsigned int);
extern void populateWithRand(int*, unsigned int, int, int);
extern void displayGameOverScore(Rect r2, int w, int h, unsigned int imageTexture, int yourCurrentScore);
extern void enemyAI(Vec trooper_pos, float trooper_angle, Vec enemy_pos, float enemy_angle, int xres, int yres);
extern void grassRazorLeaf(float, int, int);
extern void grassRazerMove(int);
extern void switchBullets(float, int, int, int, int);
extern void showAttack(int choice);
//==========================================================================
// M A I N
//==========================================================================

	X11_wrapper x11(0, 0);

int main()
{
	//set up the x11 window
	logOpen();
	init_opengl();
	srand(time(NULL));
	randomColor();
    lighting( 11, 10, 20);
	makeParticles(gl.xres, gl.yres);
	getScores(filename);
	//start the state variable
	state = GameState::startup;
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	//x11.set_mouse_position(100,100);
	int done=0;
	//creating a blender object
	//Blender obj;
	Texture hitler("images/hitler.png",0,0,0,gl.xres, gl.yres);
	

	Texture hitler_eyes_c("images/hitler_eyes_closed.png", 0 , 0 , 0 , gl.xres,gl.yres);
	Texture hitler_br("images/hitler_back_right.png",0,0,0,gl.xres,gl.yres);
	Texture hitler_sh("images/hilter_villain_shooting.png",0,0,0,gl.xres,gl.yres);
	
	Texture map("images/map.png", 0,0,0, gl.xres, gl.yres);
	Texture hud("images/hud_full_screen.png", 0,0,0, gl.xres, gl.yres);
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			done = check_keys(&e);
		}
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		timeCopy(&timeStart, &timeCurrent);
		physicsCountdown += timeSpan;
		while (physicsCountdown >= physicsRate) {
			physics();
			physicsCountdown -= physicsRate;
		}
		//lets start the game states
		switch (state){
			case GameState::startup:{
				runLogoIntro(logoIntroTexture);
				//render everything to the screen
				x11.swapBuffers();
				usleep(1000000);//sleep for 5 seconds
				state = GameState::menu;
				glClear(GL_COLOR_BUFFER_BIT);
				break;
			}
			case GameState::menu:{
				Rect r;
				//int x=200,y=200,dirX=0,dirY=0;
				int dirX=0,dirY=0;
				glClear(GL_COLOR_BUFFER_BIT);
				startMenu(r, gl.yres, gl.xres, gl.xres, gl.yres, startMenuTexture, titleImageTexture);
				makeButton(gl.xres,gl.yres,dirX,dirY);
				//changeButtonColor( gl.xres,gl.yres, dirX,dirY);
				drawLine();
				boxText(r,gl.xres,gl.yres);
				break;
			}
			case GameState::game:{
                
				//Rect r;
				
				glClear(GL_COLOR_BUFFER_BIT);
				//scoreboard(r);
				
				//glMatrixMode(GL_PROJECTION); glLoadIdentity();
				//glMatrixMode(GL_MODELVIEW); glLoadIdentity();
                //glFrustum(-gl.xres/2,gl.xres/2,-gl.yres/2,gl.yres/2, 1.0, 30);
				//map.Display_Picture(gl.xres, gl.yres, 0, -1);
				if(playerdir == 0){
					if(timeCurrent.tv_sec%5 == 0){
						hitler_eyes_c.Display_Picture(sizeX, sizeY, movex, movey-1);
					}else{
						hitler.Display_Picture(sizeX,sizeY, movex, movey-1);
					}
				}else if(playerdir == 1){
					hitler_br.Display_Picture(sizeX, sizeY, movex, movey-1);
				
				} /*Anna's addition*/
				else if(playerdir == 3) {
					//if(timeCurrent.tv_sec % 10 == 0){
					hitler_sh.Display_Picture(sizeX, sizeY, movex, movey-1);
					//}
					/*playerdir = 0;*/
				}
				render();
				//hud.Display_Picture(gl.xres, gl.yres, 0,0);
				//glMatrixMode(GL_PROJECTION); glLoadIdentity();
				//glMatrixMode(GL_MODELVIEW); glLoadIdentity();
				//glOrtho(-gl.xres/2,gl.xres/2,-gl.yres/2,gl.yres/2, -1,1);
				
				//render();
				break;
			}
			case GameState::highscores:{
				Rect r2;
				getScores(filename);
				highScoreBoard(r2, gl.xres, gl.yres, bloodBackgroundTexture);
				state = GameState::end;
				break;
			}
			case GameState::credits:{
				renderCoolCredits(gl.xres, gl.yres, imageTexture);
				glMatrixMode(GL_PROJECTION); glLoadIdentity();
				glMatrixMode(GL_MODELVIEW); glLoadIdentity();
				glOrtho(-gl.xres/2,gl.xres/2,-gl.yres/2,gl.yres/2, -1,1);
				break;
			}
			case GameState::endgamescore:{
				Rect r3;	
				displayGameOverScore(r3, gl.xres, gl.yres, imageTexture, rand()%10);
				state = GameState::end;
				break;
			}
			default:{
				break;
			}
		}
		x11.swapBuffers();
	}
	cleanup_fonts();
	logClose();
	return 0;
}

void init_opengl(void)
{
	//OpenGL initialization
	//std::cout << gl.xres << " and " << gl.yres << std::endl;
	glViewport(0, 0, gl.xres, gl.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	//glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	//glOrtho(-4,4,-4,4, -1,1);
	glOrtho(-gl.xres/2,gl.xres/2,-gl.yres/2,gl.yres/2, -1,1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//Clear the screen to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//glClearColor(0.1, 0.1, 0.1, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//Image - Background
	glGenTextures(1, &gl.backgroundTexture);
	int w = img[0].width;
	int h = img[0].height;

	glBindTexture(GL_TEXTURE_2D, gl.backgroundTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[0].data);

	img_x = (int)position.pos[0];
	img_y = (int)position.pos[1];

	imageTexture = gl.backgroundTexture;

	//Image - Start Menu Zombie
	
	glGenTextures(1, &gl.startTexture);
	int w1 = img[1].width;
	int h1 = img[1].height;

	glBindTexture(GL_TEXTURE_2D, gl.startTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w1, h1, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[1].data);

	startMenuTexture = gl.startTexture;
	
	//Image - Trooper
	glGenTextures(1, &gl.trooperTexture);
	int w2 = img[2].width;
	int h2 = img[2].height;

	glBindTexture(GL_TEXTURE_2D, gl.trooperTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w2, h2, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[2].data);

	g.trooper.trooperImageTexture = gl.trooperTexture;
	
	//Image - High scores bloodBackground
	
	glGenTextures(1, &gl.bbTexture);
	int w5 = img[5].width;
	int h5 = img[5].height;

	glBindTexture(GL_TEXTURE_2D, gl.bbTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w5, h5, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[5].data);

	bloodBackgroundTexture = gl.bbTexture;

	//Image - Skull
	
	glGenTextures(1, &gl.skullTexture);
	int w3 = img[3].width;
	int h3 = img[3].height;
	g.skull.size[0] = img[3].width;
	g.skull.size[1] = img[3].height;

	glBindTexture(GL_TEXTURE_2D, gl.skullTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w3, h3, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[3].data);

	g.skull.skullImageTexture = gl.skullTexture;

	//Image - Zombie
	
	glGenTextures(1, &gl.zombieTexture);
	int w7 = img[7].width;
	int h7 = img[7].height;
	g.zombie.size[0] = img[7].width;
	g.zombie.size[1] = img[7].height;

	glBindTexture(GL_TEXTURE_2D, gl.zombieTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w7, h7, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[7].data);

	g.zombie.zombieImageTexture = gl.zombieTexture;

	//Image - Undead Logo
	
	glGenTextures(1, &gl.logoTexture);
	int w4 = img[4].width;
	int h4 = img[4].height;

	glBindTexture(GL_TEXTURE_2D, gl.logoTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w4, h4, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[4].data);

	logoIntroTexture = gl.logoTexture;

	//Image - Undead Title
	
	glGenTextures(1, &gl.titleTexture);
	int w6 = img[6].width;
	int h6 = img[6].height;

	glBindTexture(GL_TEXTURE_2D, gl.titleTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w6, h6, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[6].data);

	titleImageTexture = gl.titleTexture;

	//Image - Water
	
	glGenTextures(1, &gl.startTexture);
	int w8 = img[8].width;
	int h8 = img[8].height;

	glBindTexture(GL_TEXTURE_2D, gl.startTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w8, h8, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[8].data);

	waterImageTexture = gl.waterTexture;
}

void normalize2d(Vec v)
{
	Flt len = v[0]*v[0] + v[1]*v[1];
	if (len == 0.0f) {
		v[0] = 1.0;
		v[1] = 0.0;
		return;
	}
	len = 1.0f / sqrt(len);
	v[0] *= len;
	v[1] *= len;
}

void check_mouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	static int ct=0;
	//std::cout << "m" << std::endl << std::flush;
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			//a little time between each bullet
			struct timespec bt;
			clock_gettime(CLOCK_REALTIME, &bt);
			double ts = timeDiff(&g.bulletTimer, &bt);
			if (ts > 0.1) {
				timeCopy(&g.bulletTimer, &bt);
				//shoot a bullet...
				if (g.nbullets < MAX_BULLETS) {
					Bullet *b = &g.barr[g.nbullets];
					timeCopy(&b->time, &bt);
					/*b->pos[0] = g.trooper.pos[0];
					b->pos[1] = g.trooper.pos[1];*/
					b->pos[0]= movex; b->pos[1] = movey;
					b->vel[0] = g.trooper.vel[0];
					b->vel[1] = g.trooper.vel[1];
					//convert trooper angle to radians
					Flt rad = ((g.trooper.angle+90.0) / 360.0f) * PI * 2.0;
					b->angle = rad;
					//convert angle to a vector
					Flt xdir = cos(rad);
					Flt ydir = sin(rad);
					// b->pos[0] += xdir*20.0f;
					// b->pos[1] += ydir*20.0f;
					b->vel[0] += xdir*6.0f;
					b->vel[1] += ydir*6.0f;
					b->pos[0] += b->vel[0];
					b->pos[1] += b->vel[1];
					
					b->row = rand() % 6;
					b->color[0] = fireColors[b->row][0];
					b->color[1] = fireColors[b->row][1];
					b->color[2] = fireColors[b->row][2];
					++g.nbullets;
				}
			}
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	//keys[XK_Up] = 0;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		int xdiff = savex - ((-2*((float)e->xbutton.x/gl.xres)+1)*gl.xres/2);
		int ydiff = savey - ((-2*((float)e->xbutton.y/gl.yres)+1)*gl.yres/2);
		if (++ct < 10)
			return;		
		// std::cout << "savex: " << savex << std::endl << std::flush;
		// std::cout << "e->xbutton.y: " << ((-2*((float)e->xbutton.y/gl.yres)+1)*gl.yres/2)<< std::endl;
		//std::flush;
		if (xdiff > 0) {
			//std::cout << "xdiff: " << xdiff << std::endl << std::flush;
			g.trooper.angle += 0.05f * (float)xdiff;
			if (g.trooper.angle >= 360.0f)
				g.trooper.angle -= 360.0f;
		}
		else if (xdiff < 0) {
			//std::cout << "xdiff: " << xdiff << std::endl << std::flush;
			g.trooper.angle += 0.05f * (float)xdiff;
			if (g.trooper.angle < 0.0f)
				g.trooper.angle += 360.0f;
		}
		if (ydiff > 0) {
			//apply thrust
			//convert trooper angle to radians
			Flt rad = ((g.trooper.angle+90.0) / 360.0f) * PI * 2.0;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			g.trooper.vel[0] += xdir * (float)ydiff * 0.01f;
			g.trooper.vel[1] += ydir * (float)ydiff * 0.01f;
			Flt speed = sqrt(g.trooper.vel[0]*g.trooper.vel[0]+
					g.trooper.vel[1]*g.trooper.vel[1]);
			if (speed > 10.0f) {
				speed = 10.0f;
				normalize2d(g.trooper.vel);
				g.trooper.vel[0] *= speed;
				g.trooper.vel[1] *= speed;
			}
			g.mouseThrustOn = true;
			clock_gettime(CLOCK_REALTIME, &g.mouseThrustTimer);
		}
		//x11.set_mouse_position(100,100);
		// savex=100;
		// savey=100;
	}
}

int check_keys(XEvent *e)
{
	//keyboard input?
	static int shift=0;
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	//Log("key: %i\n", key);
	if (e->type == KeyRelease) {
		gl.keys[key]=0;
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		return 0;
	}
	if (e->type == KeyPress) {
		//std::cout << "press" << std::endl;
		gl.keys[key]=1;
		if (key == XK_Shift_L || key == XK_Shift_R) {
			shift=1;
			return 0;
		}
	} else {
		return 0;
	}
    int choice=0;
	int dirX=0;
    int dirY=0;
	if (shift){}
	switch (key) {
		case XK_Escape:
            return 1;
			break;
		case XK_m:
			state = GameState::menu;
			break;
		case XK_space:
		    choice=2;
		    if(state != GameState::game) {
				state = GameState::game;
				changeButtonColor( gl.xres,gl.yres, dirX,dirY, choice);
				x11.swapBuffers();
		    	sleep(1);
		    }
			playerdir = 3;
		    break;
		case XK_c:
		    choice =3;
		    if(state != GameState::credits){
				state = GameState::credits;
				changeButtonColor( gl.xres,gl.yres, dirX,dirY, choice);
				x11.swapBuffers();
				sleep(1);
		    }
			break;
		case XK_h:
			state = GameState::highscores;
			break;
		case XK_g:
			state = GameState::endgamescore;
			break;
		case XK_w:
			movey++;
			playerdir = 0;
			break;
		case XK_s:
			movey--;
			playerdir = 1;
			break;
		case XK_e:
			std::cout << "Me Work" << std::endl;
			gvars::attack = (gvars::attack + 1) % 4;
			break;
		case XK_a:
			playerdir = 0;
			movex == -gl.xres/2 ? movex = gl.xres/2: movex-=5;
			sizeX = -200;
			break;
		case XK_d:
			playerdir = 0;
			movex == gl.xres/2 ? movex = -gl.xres/2: movex+=5;
			sizeX = 200;
			break;
		case XK_Down:
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;
	}
    return 0;
}

void deleteAsteroid(Game *g, Asteroid *node)
{
	//Remove a node from doubly-linked list
	//Must look at 4 special cases below.
	if (node->prev == NULL) {
		if (node->next == NULL) {
			//only 1 item in list.
			g->asteroid = NULL;
		} else {
			//at beginning of list.
			node->next->prev = NULL;
			g->asteroid = node->next;
		}
	} else {
		if (node->next == NULL) {
			//at end of list.
			node->prev->next = NULL;
		} else {
			//in middle of list.
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
	}
	delete node;
	node = NULL;
}

void buildAsteroidFragment(Asteroid *ta, Asteroid *a)
{
	//build ta from a
	ta->nverts = 8;
	ta->radius = a->radius / 2.0;
	Flt r2 = ta->radius / 2.0;
	Flt angle = 0.0f;
	Flt inc = (PI * 2.0) / (Flt)ta->nverts;
	for (int i=0; i<ta->nverts; i++) {
		ta->vert[i][0] = sin(angle) * (r2 + rnd() * ta->radius);
		ta->vert[i][1] = cos(angle) * (r2 + rnd() * ta->radius);
		angle += inc;
	}
	ta->pos[0] = a->pos[0] + rnd()*10.0-5.0;
	ta->pos[1] = a->pos[1] + rnd()*10.0-5.0;
	ta->pos[2] = 0.0f;
	ta->angle = 0.0;
	ta->rotate = a->rotate + (rnd() * 4.0 - 2.0);
	ta->color[0] = 0.8;
	ta->color[1] = 0.8;
	ta->color[2] = 0.7;
	ta->vel[0] = a->vel[0] + (rnd()*2.0-1.0);
	ta->vel[1] = a->vel[1] + (rnd()*2.0-1.0);
	//std::cout << "frag" << std::endl;
}

void physics()
{
	Flt d0,d1,dist;
	//Update trooper position
	g.trooper.pos[0] += g.trooper.vel[0];
	g.trooper.pos[1] += g.trooper.vel[1];
	//Check for collision with window edges
	if (g.trooper.pos[0] < -gl.xres/2) {
		g.trooper.pos[0] += (float)gl.xres;
	}
	else if (g.trooper.pos[0] > (float)gl.xres/2) {
		g.trooper.pos[0] -= (float)gl.xres;
	}
	else if (g.trooper.pos[1] < -gl.yres/2) {
		g.trooper.pos[1] += (float)gl.yres;
	}
	else if (g.trooper.pos[1] > (float)gl.yres/2) {
		g.trooper.pos[1] -= (float)gl.yres;
	}
	//Update bullet positions
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	int i=0;
	while (i < g.nbullets) {
		Bullet *b = &g.barr[i];
		//How long has bullet been alive?
		double ts = timeDiff(&b->time, &bt);
		if (ts > 2.5) {
			//time to delete the bullet.
			memcpy(&g.barr[i], &g.barr[g.nbullets-1],
				sizeof(Bullet));
			g.nbullets--;
			//do not increment i.
			continue;
		}
		//move the bullet
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];
		//Check for collision with window edges
		if (b->pos[0] < -gl.xres/2) {
			b->pos[0] += (float)gl.xres;
		}
		else if (b->pos[0] > (float)gl.xres/2) {
			b->pos[0] -= (float)gl.xres;
		}
		else if (b->pos[1] < -gl.yres/2) {
			b->pos[1] += (float)gl.yres;
		}
		else if (b->pos[1] > (float)gl.yres/2) {
			b->pos[1] -= (float)gl.yres;
		}
		i++;
	}
	//
	//Update asteroid positions
	Asteroid *a = g.asteroid;
	while (a) {
		a->pos[0] += a->vel[0];
		a->pos[1] += a->vel[1];
		//Check for collision with window edges
		if (a->pos[0] < -gl.xres/2) {
			a->pos[0] += (float)gl.xres;
		}
		else if (a->pos[0] > (float)(gl.xres/2)) {
			a->pos[0] -= (float)gl.xres;
		}
		else if (a->pos[1] < -gl.yres/2) {
			a->pos[1] += (float)gl.yres;
		}
		else if (a->pos[1] > (float)(gl.yres/2)) {
			a->pos[1] -= (float)gl.yres;
		}
		a->angle += a->rotate;
		a = a->next;
	}
	//
	//Asteroid collision with bullets?
	//If collision detected:
	//     1. delete the bullet
	//     2. break the asteroid into pieces
	//        if asteroid small, delete it
	a = g.asteroid;
	while (a) {
		//is there a bullet within its radius?
		int i=0;
		while (i < g.nbullets) {
			Bullet *b = &g.barr[i];
			d0 = b->pos[0] - a->pos[0];
			d1 = b->pos[1] - a->pos[1];
			dist = (d0*d0 + d1*d1);
			if (dist < (a->radius*a->radius)) {
				//std::cout << "asteroid hit." << std::endl;
				//this asteroid is hit.
				if (a->radius > MINIMUM_ASTEROID_SIZE) {
					//break it into pieces.
					Asteroid *ta = a;
					buildAsteroidFragment(ta, a);
					int r = rand()%10+5;
					for (int k=0; k<r; k++) {
						//get the next asteroid position in the array
						Asteroid *ta = new Asteroid;
						buildAsteroidFragment(ta, a);
						//add to front of asteroid linked list
						ta->next = g.asteroid;
						if (g.asteroid != NULL)
							g.asteroid->prev = ta;
						g.asteroid = ta;
						g.nasteroids++;
					}
				} else {
					a->color[0] = 1.0;
					a->color[1] = 0.1;
					a->color[2] = 0.1;
					//asteroid is too small to break up
					//delete the asteroid and bullet
					Asteroid *savea = a->next;
					deleteAsteroid(&g, a);
					a = savea;
					g.nasteroids--;
				}
				//delete the bullet...
				memcpy(&g.barr[i], &g.barr[g.nbullets-1], sizeof(Bullet));
				g.nbullets--;
				if (a == NULL)
					break;
			}
			i++;
		}
		if (a == NULL)
			break;
		a = a->next;
	}
	//---------------------------------------------------
	//check keys pressed now
	if (gl.keys[XK_Left]) {
		g.trooper.angle += 4.0;
		if (g.trooper.angle >= 360.0f)
			g.trooper.angle -= 360.0f;
	}
	if (gl.keys[XK_Right]) {
		g.trooper.angle -= 4.0;
		if (g.trooper.angle < 0.0f)
			g.trooper.angle += 360.0f;
	}
	if (gl.keys[XK_Up]) {
		//apply thrust
		//convert trooper angle to radians
		Flt rad = ((g.trooper.angle+90.0) / 360.0f) * PI * 2.0;

		//convert angle to a vector
		Flt xdir = cos(rad);
		Flt ydir = sin(rad);
		g.trooper.vel[0] += xdir*0.02f;
		g.trooper.vel[1] += ydir*0.02f;
		Flt speed = sqrt(g.trooper.vel[0]*g.trooper.vel[0]+
				g.trooper.vel[1]*g.trooper.vel[1]);
		if (speed > 10.0f) {
			speed = 10.0f;
			normalize2d(g.trooper.vel);
			g.trooper.vel[0] *= speed;
			g.trooper.vel[1] *= speed;
		}
	}
	if (gl.keys[XK_space]) {
		//a little time between each bullet
		struct timespec bt;
		clock_gettime(CLOCK_REALTIME, &bt);
		double ts = timeDiff(&g.bulletTimer, &bt);
		if (ts > 0.1) {
			timeCopy(&g.bulletTimer, &bt);
			if (g.nbullets < MAX_BULLETS) {
				//shoot a bullet...
				//Bullet *b = new Bullet;
				Bullet *b = &g.barr[g.nbullets];
				timeCopy(&b->time, &bt);
				b->pos[0] = g.trooper.pos[0];
				b->pos[1] = g.trooper.pos[1];
				b->vel[0] = g.trooper.vel[0];
				b->vel[1] = g.trooper.vel[1];
				//convert trooper angle to radians
				Flt rad = g.trooper.angle* PI; //((g.trooper.angle+90.0) / 360.0f) * PI * 2.0;
				b->angle = rad;
				//convert angle to a vector
				Flt xdir = cos(rad);
				Flt ydir = sin(rad);
				b->pos[0] += xdir;
				b->pos[1] += ydir;
				b->vel[0] += xdir*6.0f;
				b->vel[1] += ydir*6.0f;
				
				
				/*FIRE BULLETS
				
				b->row = rand() % 6;
				b->color[0] = fireColors[b->row][0];
				b->color[1] = fireColors[b->row][1];
				b->color[2] = fireColors[b->row][2];
				*/
				//Bullets for all
				/*
				b->row = 1;
				b->color[0] = lightningColors[b->row][0];
				b->color[1] = lightningColors[b->row][1];
				b->color[2] = lightningColors[b->row][2];
				*/
				g.nbullets++;
			}
		}
	}
	if (g.mouseThrustOn) {
		//should thrust be turned off
		struct timespec mtt;
		clock_gettime(CLOCK_REALTIME, &mtt);
		double tdif = timeDiff(&mtt, &g.mouseThrustTimer);
		//std::cout << "tdif: " << tdif << std::endl;
		if (tdif < -0.3)
			g.mouseThrustOn = false;
	}
}

void render()
{ 
	Texture zombie("images/zombie.png",0,0,0,gl.xres,gl.yres);
	showAttack(gvars::attack);
    /*
	Anna Commented
	Rect r;
	glClear(GL_COLOR_BUFFER_BIT);
  
	r.bot = gl.yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, "3350 - Asteroids");
//	ggprint8b(&r, 16, 0x00ffff00, "n bullets: %i", g.nbullets);
//	ggprint8b(&r, 16, 0x00ffff00, "n asteroids: %i", g.nasteroids);
    creditManvir(r);
	ggprint8b(&r, 16, 0x00ffff00, "\n");
    creditsAnna(r);
	ggprint8b(&r, 16, 0x00ffff00, "\n");
    creditsGerardo(r);
	ggprint8b(&r, 16, 0x00ffff00, "\n");
    creditsKevin(r);



	for(int i = 0; i< gvars::MAX_SKULLS;i++){		
		Skull *s = &g.skulls[i];		
		movingImages(g.skull.size[0] / 2 + g.skull.size[0] / 4, g.skull.size[0] / 2 + g.skull.size[0] / 4, s->pos, s->angle, g.skull.skullImageTexture);		
		enemyAI(g.trooper.pos, g.trooper.angle, s->pos, s->angle, gl.xres, gl.yres);		

 	}
	//-------------------------------------------------------------------------
	//Draw the Zombies and Skulls
	//for(int i = 0; i < 3; i++)
	//	g.zombie.pos[i] = g.zombie.pos[i] + 300.0;
*/
	zombie.Display_Picture(g.zombie.size[0] / 3, g.zombie.size[0] / 3, *(g.zombie.pos), g.zombie.angle);

	//movingImages(g.zombie.size[0], g.zombie.size[0], g.zombie.pos,
		//g.zombie.angle, zombie.getID());
	enemyAI(g.trooper.pos, g.trooper.angle, g.zombie.pos, g.zombie.angle, 	
		gl.xres, gl.yres);
/*
Anna Commented
	movingImages(g.skull.size[0] / 2 + g.skull.size[0] / 4, g.skull.size[0] / 2 
		+ g.skull.size[0] / 4, g.skull.pos, g.skull.angle, g.skull.skullImageTexture);
	enemyAI(g.trooper.pos, g.trooper.angle, g.skull.pos, g.skull.angle, gl.xres, gl.yres);
*/
	/*g.enemy.pos[0] += g.trooper.vel[0] * 1.2;
	g.enemy.pos[1] += g.trooper.vel[1] * 1.2;*/

	//-------------------------------------------------------------------------
/*
Anna commented
	movingImages(50, 50, g.trooper.pos, g.trooper.angle, g.trooper.trooperImageTexture);
	Flt rad;
	if (gl.keys[XK_Up] || g.mouseThrustOn) {
		int i;
		//draw thrust
		rad = ((g.trooper.angle+90.0) / 360.0f) * PI * 2.0;
	
		//convert angle to a vector
		Flt xdir = cos(rad);
		Flt ydir = sin(rad);
		Flt xs,ys,xe,ye,r;
		glBegin(GL_LINES);
		for (i=0; i<16; i++) {
			xs = -xdir * 11.0f + rnd() * 4.0 - 2.0;
			ys = -ydir * 11.0f + rnd() * 4.0 - 2.0;
			r = rnd()*40.0+40.0;
			xe = -xdir * r + rnd() * 18.0 - 9.0;
			ye = -ydir * r + rnd() * 18.0 - 9.0;
			glColor3f(rnd()*.3+.7, rnd()*.3+.7, 0);
			glVertex2f(g.trooper.pos[0]+xs,g.trooper.pos[1]+ys);
			glVertex2f(g.trooper.pos[0]+xe,g.trooper.pos[1]+ye);
		}
		glEnd();
	}


	//-------------------------------------------------------------------------
	//Draw the asteroids
	{
		Asteroid *a = g.asteroid;
		while (a) {
			
			//Log("draw asteroid...\n");
			glColor3fv(a->color);
			glPushMatrix();
			glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
			glRotatef(a->angle, 0.0f, 0.0f, 1.0f);
			glLineWidth(1);
			glBegin(GL_LINE_LOOP);
			//Log("%i verts\n",a->nverts);
			for (int j=0; j<a->nverts; j++) {
				glVertex2f(a->vert[j][0], a->vert[j][1]);
			}
			glEnd();
			//glBegin(GL_LINES);
			//	glVertex2f(0,   0);
			//	glVertex2f(a->radius, 0);
			//glEnd();
			glPopMatrix();
			glColor3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_POINTS);
			glVertex2f(a->pos[0], a->pos[1]);
			glEnd();
			a = a->next;
			
			*/
			/*movingImages(30, 30, a->pos, a->angle, g.villain.skullImageTexture);*/
			/*
			Anna Commented
		}
	}*/
	//-------------------------------------------------------------------------
	//Draw the bullets
	
	for (int i=0; i<g.nbullets; i++) {
		Bullet *b = &g.barr[i];
		/*//Log("draw bullet...\n");
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_POINTS);
		glVertex2f(b->pos[0],      b->pos[1]);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]);
		glVertex2f(b->pos[0],      b->pos[1]-1.0f);
		glVertex2f(b->pos[0],      b->pos[1]+1.0f);
		glColor3f(0.8, 0.8, 0.8);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
		glEnd();*/
		
		//fireCircles(b->row, b->pos[0], b->pos[1]);
		
		//lightningShoots(b->angle, b->pos[0], b->pos[1]);
		//grassVines(b->angle, b->pos[0], b->pos[1]);

		//waterBubbles(b->pos[0], b->pos[1]);
		
		//b->pos[0] += 10;
		//b->pos[1] += 10;
		int choice = gvars::attack;		
		switchBullets(b->angle, b->row, b->pos[0], b->pos[1], choice);
		//fireCircles(b->row, b->pos[0], b->pos[1]);
		//b->pos[1] += .25;

		//std::cout << "I am poo poo " << std::endl;

	}
    
}
