//
//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014 - 2018
//mod spring 2015: added constructors
//This program is a game starting point for a 3350 project.

#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <thread>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/glu.h>
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


using namespace gvars;

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
		"./images/background.png",
		"./images/ghost_skull.png",
		"./images/undead_logo.png",
		"./images/bloodBackground.png",
		"./images/curse_of_undead.png",
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
//extern void drawVine();
//extern void drawLine();
extern void scoreboard(Rect r);
extern void lightningShoots(float, int, int);
extern void fireCircles(int, int, int);
extern void grassVines(float, int, int);
extern void waterBubbles(int, int);
extern void boxText(Rect r, int, int );
extern void characterText(Rect r, int, int );
extern void lighting( int size, int start, int end);
extern void runLogoIntro(unsigned int logoIntroTexture);
extern void changeButtonColor( int , int ,int dirX, int dirY, int choice);
extern void DisplayGameControls(Rect r);
extern void characterChoice();
extern void characterOption(int option);
//extern void renderVine(int size, int start, int end);
extern void highScoreBoard(Rect, int, int, unsigned int);
extern void populateWithRand(int*, unsigned int, int, int);
extern void displayGameOverScore(Rect r2, int w, int h, unsigned int imageTexture, int yourCurrentScore);
extern void skullAI(Vec enemy_pos, int xres, int yres);
extern void zombieAI(Vec trooper_pos, float trooper_angle, Vec enemy_pos, float enemy_angle, int xres, int yres);
extern void grassRazorLeaf(float, int, int);
extern void grassRazerMove(int);
extern void switchBullets(float, int, int, int, int);
extern void showAttack(int choice);
extern void isometricScene(); 
extern void dyingAnimation(Vec enemy_pos);
extern void fireballAttack(int * fire_pos);
extern void flicker(int * fire_pos);
extern void drawCircle(float cx, float cy, float r, int num_segments);
extern GLvoid draw_circle(const GLfloat radius,const GLuint num_vertex);
extern void orthoScene();
extern void arrowInputMap(XEvent *);
extern void bulletsTravel(float * pos, int dir);
extern void checkBulletCollision(Bullet *b, int & nbullets);
extern void showHud();
extern void checkZombieCollision(Zombie *zs, int zcount);
extern void checkSkullCollision(Skull *zs, int zcount);
extern void bulletEnemyCollision(Vec enemyPos, Vec bulletPos);
extern void music();
extern int stop;
//==========================================================================
// M A I N
//==========================================================================

X11_wrapper x11(0, 0);
int musicstart = 0;
//std::thread kmusic(music);
int main()
{
	//set up the x11 window
	logOpen();
	init_opengl();
	srand(time(NULL));
	randomColor();

	makeParticles(gl.xres, gl.yres);
	getScores(filename);
	//start the state variable
	state = GameState::startup;
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);

	int done=0;
	//remove later
	Rect r3;
	displayGameOverScore(r3, gl.xres, gl.yres, imageTexture, 
						rand()%20);
	//up to here ask manvir for removal

	
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

		//lets start the game states
		switch (state){
			case GameState::startup:{
				glClear(GL_COLOR_BUFFER_BIT);
				runLogoIntro(logoIntroTexture);
				//render everything to the screen
				x11.swapBuffers();
				usleep(100000);//sleep for 5 seconds
				state = GameState::menu;
				glClear(GL_COLOR_BUFFER_BIT);
				break;
			}
			case GameState::menu:{
				Rect r;
				//int x=200,y=200,dirX=0,dirY=0;
				int dirX=0,dirY=0;
				if(stop){
					kill(stop, 9);
				}
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				orthoScene();
				startMenu(r, gl.yres, gl.xres, gl.xres, gl.yres,
								 startMenuTexture, titleImageTexture);
				makeButton(gl.xres,gl.yres,dirX,dirY);
				//changeButtonColor( gl.xres,gl.yres, dirX,dirY);
				//drawLine();
			//	drawVine();
                boxText(r,gl.xres,gl.yres);
				break;
			}

            case GameState::characterSelect:{
				//int x=200,y=200,dirX=0,dirY=0;
				//int dirX=0,dirY=0;
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				orthoScene();
				//startMenu(r, gl.yres, gl.xres, gl.xres, gl.yres,
				//				 startMenuTexture, titleImageTexture);
				//makeButton(gl.xres,gl.yres,dirX,dirY);
				//changeButtonColor( gl.xres,gl.yres, dirX, dirY);
				//drawLine();
				//drawVine();
                characterChoice();
                characterOption(hero.character_option);
				hero.characterChange();
				
				break;
			}
            case GameState::howToPlay:{
                Rect r;
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				orthoScene();
                DisplayGameControls(r);
				break;
			}
			case GameState::game:{
                
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

				std::cout << "Health: " << hero.lifeForce << std::endl;
				showHud();
				
				hero.recovery(timeCurrent.tv_sec);
				
				map.Display_Picture(gl.xres/2,gl.yres/2,0,0);
				if(hero.face){
					map_1.Display_Picture(gl.xres/2,gl.yres/2,0,0);
					hero.characterRender();
				}else{		
					hero.characterRender();
					map_1.Display_Picture(gl.xres/2,gl.yres/2,0,0);
				}

				render();
				break;
			}
			case GameState::highscores:{
				Rect r2;
				glClear(GL_COLOR_BUFFER_BIT);
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				orthoScene();
				getScores(filename);
				highScoreBoard(r2, gl.xres, gl.yres, bloodBackgroundTexture);				
				break;
			}
			case GameState::credits:{
				// if(musicstart == 0){
					
				// 	std::thread kmusic(music);
					
				// 	kmusic.detach();
				// 	musicstart = 1;
					
				// }
				renderCoolCredits(gl.xres, gl.yres, imageTexture);
				glMatrixMode(GL_PROJECTION); glLoadIdentity();
				glMatrixMode(GL_MODELVIEW); glLoadIdentity();
				glOrtho(-gl.xres/2,gl.xres/2,-gl.yres/2,gl.yres/2, -1,1);
				
				break;
			}
			case GameState::endgamescore:{
				Rect r3;	
				glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
				orthoScene();
				displayGameOverScore(r3, gl.xres, gl.yres, imageTexture, 
										rand()%10);
				hero.reset();
				tp.reset();
				g.reset();
				init_opengl();
				state = GameState::end;
				break;
			}
			case GameState::end:
				break;
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

	glBindTexture(GL_TEXTURE_2D, gl.skullTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w3, h3, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[3].data);

	for(int i = 0; i < g.skullcount; i++){
		g.skull[i].skullImageTexture = gl.skullTexture;
		g.skull[i].size[0] = img[3].width;
		g.skull[i].size[1] = img[3].height;
	}

	//Image - Zombie
	
	glGenTextures(1, &gl.zombieTexture);
	int w7 = img[7].width;
	int h7 = img[7].height;

	glBindTexture(GL_TEXTURE_2D, gl.zombieTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w7, h7, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[7].data);

	for(int i = 0; i < g.zombiecount; i++){
		g.zombie[i].zombieImageTexture = gl.zombieTexture;
		g.zombie[i].size[0] = img[7].width;
		g.zombie[i].size[1] = img[7].height;
	}
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

					b->pos[0] = hero.pos[0];
					b->pos[1] = hero.pos[2];
					b->type = gvars::attack;
					b->angle = (int)hero.dir;
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
	arrowInputMap(e);
	if (e->type == KeyRelease) {
		gl.keys[key]=0;
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		return 0;
	}
	if (e->type == KeyPress) {
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
		    if(state == GameState::menu) {
				state = GameState::game;
				changeButtonColor( gl.xres,gl.yres, dirX,dirY, choice);
				x11.swapBuffers();
		    	sleep(1);
		    }
			hero.setFace(7);
		    break;
		case XK_c:
		    choice =3;
		    if(state == GameState::menu){
				state = GameState::credits;
				changeButtonColor( gl.xres,gl.yres, dirX,dirY, choice);
				x11.swapBuffers();
				sleep(1);
		    }
			break;
		case XK_t:
            choice =1;
		    if(state == GameState::menu){
				state = GameState::characterSelect;
				changeButtonColor( gl.xres,gl.yres, dirX,dirY, choice);
				x11.swapBuffers();
				sleep(1);
		    }
			break;
        case XK_1:
		    hero.character_option =1;
		    if(state == GameState::menu){
				state = GameState::characterSelect;
		    }
			break;
        case XK_2:
		    hero.character_option =2;
		    if(state == GameState::menu){
				state = GameState::characterSelect;
		    }
			break;
        case XK_3:
		    hero.character_option =3;
		    if(state == GameState::menu){
				state = GameState::characterSelect;
		    }
			break;
		case XK_j:
            choice=4;
		    if(state == GameState::menu){
				state = GameState::howToPlay;
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
		case XK_e:
			if(state == GameState::game)
				gvars::attack = ((gvars::attack + 1) % 4);
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;
	}
    return 0;
}

Texture zombie("images/zombie.png",0,0,0,gl.xres,gl.yres);
Texture skull("images/skull.png",0,0,0,gl.xres,gl.yres);
void render()
{ 
	showAttack(gvars::attack);


	for(int i = 0; i < g.zombiecount; i++){
		zombie.Display_Picture(g.zombie[i].size[0] / 30, g.zombie[i].size[0] / 30,   
					g.zombie[i].pos[0], g.zombie[i].pos[1]);
		skullAI(g.zombie[i].pos, gl.xres, gl.yres);
		checkZombieCollision(g.zombie, g.zombiecount);
		

	}

	for(int i = 0; i < g.skullcount; i++){
		skull.Display_Picture(g.skull[i].size[0] / 20, g.skull[i].size[0] / 20,   
					g.skull[i].pos[0], g.skull[i].pos[1]);
		skullAI(g.skull[i].pos, gl.xres, gl.yres);
		checkSkullCollision(g.skull, g.skullcount);
	}

	checkBulletCollision(g.barr, g.nbullets);
	for (int i=0; i<g.nbullets; i++) {
		Bullet *b = &g.barr[i];
		bulletsTravel(b->pos, b->angle);
		switchBullets(b->angle, b->row, b->pos[0]+4, b->pos[1]+8, b->type);
		for(int j = 0; j < g.zombiecount; j++){
			bulletEnemyCollision(g.zombie[i].pos, b->pos);
			bulletEnemyCollision(g.skull[i].pos, b->pos);
		}
	}
    
}
