//
//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014 - 2018
//mod spring 2015: added constructors
//This program is a game starting point for a 3350 project.
//
//
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
#include <fstream>
#include <cstring>

//Include Files for Classes
#include "Image.h"
#include "Global.h"
#include "Trooper.h"
#include "Villain.h"
#include "Bullet.h"


//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];

int img_x;
int img_y;
//image for background
GLuint imageTexture; 
//image for zombie
GLuint startMenuTexture; 
//image for trooper
GLuint trooperImageTexture; 
//image for asteroid
GLuint villainImageTexture; 
//File for Reading In HighScore
char filename[] = "scores.txt";

int yourCurrentScore =0;
//These are Flags for the game.
int credits = 0;
int highScore = 0;
int gameOver = 0;

//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
						(c)[1]=(a)[1]-(b)[1]; \
						(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1
const int MAX_BULLETS = 11;
const Flt MINIMUM_ASTEROID_SIZE = 60.0;

//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
int started = 0;
int doneStart = 0;
int changeColor=0;
int grabHighScores=0;
extern struct timespec timeStart, timeCurrent;
extern struct timespec timePause;
extern double physicsCountdown;
extern double timeSpan;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------
// Set Up Images
Global gl;
Image img[5] = {
		"./images/background.png",
		"./images/zombie_start.png",
		"./images/trooper.png",
		"./images/villain.png",
		"./images/undead_logo.png"
};

class Position {
public:
	Vec pos;
	Vec vel;
} position;

class Asteroid {
public:
	Vec pos;
	Vec vel;
	int nverts;
	Flt radius;
	Vec vert[8];
	float angle;
	float rotate;
	float color[3];
	struct Asteroid *prev;
	struct Asteroid *next;
public:
	Asteroid() {
		prev = NULL;
		next = NULL;
	}
};


class Game {
public:
	Trooper trooper;
	Asteroid *asteroid;
	Bullet *barr;
	int nasteroids;
	int nbullets;
	struct timespec bulletTimer;
	struct timespec mouseThrustTimer;
	bool mouseThrustOn;
public:
	Game() {
		asteroid = NULL;
		barr = new Bullet[MAX_BULLETS];
		nasteroids = 0;
		nbullets = 0;
		mouseThrustOn = false;
		//build 10 asteroids...
		for (int j=0; j<10; j++) {
			Asteroid *a = new Asteroid;
			a->nverts = 8;
			a->radius = rnd()*80.0 + 40.0;
			Flt r2 = a->radius / 2.0;
			Flt angle = 0.0f;
			Flt inc = (PI * 2.0) / (Flt)a->nverts;
			for (int i=0; i<a->nverts; i++) {
				a->vert[i][0] = sin(angle) * (r2 + rnd() * a->radius);
				a->vert[i][1] = cos(angle) * (r2 + rnd() * a->radius);
				angle += inc;
			}
			a->pos[0] = (2*((float)rand() / RAND_MAX)-1)*(gl.xres/2);
			a->pos[1] = (2*((float)rand() / RAND_MAX)-1)*(gl.yres/2);
			a->pos[2] = 0.0f;
			a->angle = 0.0f;
			a->rotate = rnd() * 4.0 - 2.0;
			a->color[0] = 0.8;
			a->color[1] = 0.8;
			a->color[2] = 0.7;
			a->vel[0] = (Flt)(rnd()*2.0-1.0);
			a->vel[1] = (Flt)(rnd()*2.0-1.0);
			//std::cout << "asteroid" << std::endl;
			//add to front of linked list
			a->next = asteroid;
			if (asteroid != NULL)
				asteroid->prev = a;
			asteroid = a;
			++nasteroids;
		}
		clock_gettime(CLOCK_REALTIME, &bulletTimer);
	}
	~Game() {
		delete [] barr;
	}
} g;

//X Windows variables
class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
public:
	X11_wrapper() { }
	X11_wrapper(int w, int h) {
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		XSetWindowAttributes swa;
		setup_screen_res(gl.xres, gl.yres);
		dpy = XOpenDisplay(NULL);
		if (dpy == NULL) {
			std::cout << "\n\tcannot connect to X server" << std::endl;
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XWindowAttributes getWinAttr;
		XGetWindowAttributes(dpy, root, &getWinAttr);
		int fullscreen=0;
		gl.xres = w;
		gl.yres = h;
		if (!w && !h) {
			//Go to fullscreen.
			//std::cout << "here" << std::endl;
			gl.xres = getWinAttr.width;
			gl.yres = getWinAttr.height;
			//When window is fullscreen, there is no client window
			//so keystrokes are linked to the root window.
			//XGrabKeyboard(dpy, root, False,
				//GrabModeAsync, GrabModeAsync, CurrentTime);
			fullscreen=1;
		}
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if (vi == NULL) {
			std::cout << "\n\tno appropriate visual found\n" << std::endl;
			exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		swa.colormap = cmap;
		swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
			PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |
			StructureNotifyMask | SubstructureNotifyMask;
		unsigned int winops = CWBorderPixel|CWColormap|CWEventMask;
		if (fullscreen) {
			//winops |= CWOverrideRedirect;
			//swa.override_redirect = True;
		}
		win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
			vi->depth, InputOutput, vi->visual, winops, &swa);
		//win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
		//vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
		set_title();
		glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
		show_mouse_cursor(0);
	}
	~X11_wrapper() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	void set_title() {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "Asteroids template");
	}
	void check_resize(XEvent *e) {
		//The ConfigureNotify is sent by the
		//server if the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		if (xce.width != gl.xres || xce.height != gl.yres) {
			//Window size did change.
			reshape_window(xce.width, xce.height);
		}
	}
	void reshape_window(int width, int height) {
		//window has been resized.
		setup_screen_res(width, height);
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		
		glOrtho(-gl.xres/2,gl.xres/2,-gl.yres/2,gl.yres/2, -1,1);

		set_title();
	}
	void setup_screen_res(const int w, const int h) {
		gl.xres = w;
		gl.yres = h;
	}
	void swapBuffers() {
		glXSwapBuffers(dpy, win);
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void set_mouse_position(int x, int y) {
		XWarpPointer(dpy, None, win, 0, 0, 0, 0, x, y);
	}
	void show_mouse_cursor(const int onoff) {
		if (onoff) {
			//this removes our own blank cursor.
			XUndefineCursor(dpy, win);
			return;
		}
		//vars to make blank cursor
		Pixmap blank;
		XColor dummy;
		char data[1] = {0};
		Cursor cursor;
		//make a blank cursor
		blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
		if (blank == None)
			std::cout << "error: out of memory." << std::endl;
		cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
		XFreePixmap(dpy, blank);
		//this makes you the cursor. then set it using this function
		XDefineCursor(dpy, win, cursor);
		//after you do not need the cursor anymore use this function.
		//it will undo the last change done by XDefineCursor
		//(thus do only use ONCE XDefineCursor and then XUndefineCursor):
	}
} x11(0, 0);



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
extern void startMenu(Rect r, int y_num, int x_num, int img_x, int img_y, GLuint startMenuTexture);
extern void movingImages(int width_x, int height_y, Vec img_pos, float img_angle, GLuint texture);
extern void randomColor();
extern void renderCoolCredits(int w, int h, GLuint imageTexture);
extern void makeParticles(int, int);
extern void getScores(char*, int &);
extern void makeButton(int x, int y, int dirX, int dirY);
extern void highScoreBoard(Rect r, int w, int h, GLuint imageTexture);
extern void boxText(Rect r);
extern void changeButtonColor( int y, int x,int dirX, int dirY, int &doneStart);
extern void displayGameOverScore(Rect r2, int w, int h, GLuint imageTexture, int yourCurrentScore);
//==========================================================================
// M A I N
//==========================================================================


int main()
{
	logOpen();
	init_opengl();
	srand(time(NULL));
	randomColor();
	makeParticles(gl.xres, gl.yres);
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	//x11.set_mouse_position(100,100);
	int done=0;

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
		if(!grabHighScores){
			getScores(filename,grabHighScores);
		}
		/*Loading Starting Intro
         *Passing In Parameters
         */
		
        if(!started) {
            Rect r;
            int x=200;
            int y=200;
            int dirX=0;
            int dirY=0;
	        glClear(GL_COLOR_BUFFER_BIT);
            startMenu(r, gl.yres, gl.xres, gl.xres, gl.yres, startMenuTexture);
            makeButton(x,y,dirX,dirY);
            boxText(r);
        }
        else {
			if(gameOver){
				Rect r3;
				displayGameOverScore(r3, gl.xres, gl.yres, imageTexture, yourCurrentScore);
			}
			else if(credits){
				renderCoolCredits(gl.xres, gl.yres, imageTexture);
				glMatrixMode(GL_PROJECTION); glLoadIdentity();
				glMatrixMode(GL_MODELVIEW); glLoadIdentity();
				glOrtho(-gl.xres/2,gl.xres/2,-gl.yres/2,gl.yres/2, -1,1);
			}
			else if(highScore){
				Rect r2;
				//getScores(filename, grabHighScores);
				highScoreBoard(r2, gl.xres, gl.yres, imageTexture);
			}
            else if(doneStart == 1){
                //Rect r;
                int dirX=0;
                int dirY=0;
                int x=200;
                int y=200;
                changeButtonColor( y, x, dirX,dirY, doneStart);
            }
            else{
		    	render();
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
	//
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
	//
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
	int w2 = img[1].width;
	int h2 = img[1].height;

	glBindTexture(GL_TEXTURE_2D, gl.startTexture);
	//
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w2, h2, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[1].data);

	startMenuTexture = gl.startTexture;
	
	//Image - Trooper
	
	glGenTextures(1, &gl.trooperTexture);
	int w3 = img[2].width;
	int h3 = img[2].height;

	glBindTexture(GL_TEXTURE_2D, gl.trooperTexture);
	//
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w3, h3, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[2].data);

	trooperImageTexture = gl.trooperTexture;
	
	//Image - Asteroid
	
	glGenTextures(1, &gl.villainTexture);
	int w4 = img[3].width;
	int h4 = img[3].height;

	glBindTexture(GL_TEXTURE_2D, gl.villainTexture);
	//
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w4, h4, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[3].data);

	villainImageTexture = gl.villainTexture;
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
					b->pos[0] = g.trooper.pos[0];
					b->pos[1] = g.trooper.pos[1];
					b->vel[0] = g.trooper.vel[0];
					b->vel[1] = g.trooper.vel[1];
					//convert trooper angle to radians
					Flt rad = ((g.trooper.angle+90.0) / 360.0f) * PI * 2.0;
					//convert angle to a vector
					Flt xdir = cos(rad);
					Flt ydir = sin(rad);
					b->pos[0] += xdir*20.0f;
					b->pos[1] += ydir*20.0f;
					b->vel[0] += xdir*6.0f + rnd()*0.1;
					b->vel[1] += ydir*6.0f + rnd()*0.1;
					b->color[0] = 1.0f;
					b->color[1] = 1.0f;
					b->color[2] = 1.0f;
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
	if (shift){}
	switch (key) {
		case XK_Escape:
            return 1;
			break;
        case XK_space:
            started = 1;
            doneStart = 1;
            break;
		case XK_c:
			credits ^= 1;
			break;
		case XK_h:
			highScore ^= 1;
			break;
		case XK_g:
			gameOver ^= 1;
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
				Flt rad = ((g.trooper.angle+90.0) / 360.0f) * PI * 2.0;
				//convert angle to a vector
				Flt xdir = cos(rad);
				Flt ydir = sin(rad);
				b->pos[0] += xdir*20.0f;
				b->pos[1] += ydir*20.0f;
				b->vel[0] += xdir*6.0f + rnd()*0.1;
				b->vel[1] += ydir*6.0f + rnd()*0.1;
				b->color[0] = 1.0f;
				b->color[1] = 1.0f;
				b->color[2] = 1.0f;
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
    Rect r;
	glClear(GL_COLOR_BUFFER_BIT);
  
	r.bot = gl.yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, "3350 - Asteroids");
	ggprint8b(&r, 16, 0x00ffff00, "n bullets: %i", g.nbullets);
	ggprint8b(&r, 16, 0x00ffff00, "n asteroids: %i", g.nasteroids);
    creditManvir(r);
	ggprint8b(&r, 16, 0x00ffff00, "\n");
    creditsAnna(r);
	ggprint8b(&r, 16, 0x00ffff00, "\n");
    creditsGerardo(r);
	ggprint8b(&r, 16, 0x00ffff00, "\n");
    creditsKevin(r);
	//-------------------------------------------------------------------------
	//Draw the trooper
	/*glColor3fv(g.trooper.color);
	glPushMatrix();
	glTranslatef(g.trooper.pos[0], g.trooper.pos[1], g.trooper.pos[2]);
	glTranslatef(0, 0, 0);
	
	//float angle = atan2(trooper.dir[1], trooper.dir[0]);
	glRotatef(g.trooper.angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	//glVertex2f(-10.0f, -10.0f);
	//glVertex2f(  0.0f, 20.0f);
	//glVertex2f( 10.0f, -10.0f);
	glVertex2f(-12.0f, -10.0f);
	glVertex2f(  0.0f, 20.0f);
	glVertex2f(  0.0f, -6.0f);
	glVertex2f(  0.0f, -6.0f);
	glVertex2f(  0.0f, 20.0f);
	glVertex2f( 12.0f, -10.0f);
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2f(0.0f, 0.0f);
	glEnd();
	glPopMatrix();*/
	movingImages(30, 30, g.trooper.pos, g.trooper.angle, trooperImageTexture);
	
	if (gl.keys[XK_Up] || g.mouseThrustOn) {
		int i;
		//draw thrust
		Flt rad = ((g.trooper.angle+90.0) / 360.0f) * PI * 2.0;
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
			
			/*
			movingImages(30, 30, a->pos, a->angle, villainImageTexture);
			*/
		}
	}
	//-------------------------------------------------------------------------
	//Draw the bullets
	for (int i=0; i<g.nbullets; i++) {
		Bullet *b = &g.barr[i];
		//Log("draw bullet...\n");
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
		glEnd();
	}
    
}
