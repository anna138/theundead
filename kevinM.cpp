// Modified by: Kevin Mitra
// CMPS 3350
// Modified Date: 10/11/2019

/* Steps for Debugging code
 * 1. Reproduce the bug before changing code
 * 2. Understand stack traces
 * 3. Write a test case that Reproduces the bug
 * 4. Know your error codes
 */

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/glut.h>
#include "fonts.h"
#include <math.h>

using namespace std;

void makeParticle(int a, int b);
void creditsKevin(Rect r);

struct Vec 
{
    float x, y, z;
}; 

const int MAX_PARTICLES = 15000;

struct Shape 
{
    float width = 80, height = 10;
    float radius;
    Vec center;
};

struct Particle 
{
    Shape s;
    Vec velocity;
};

class MusicT 
{

};

void makeParticle(int x, int y)
{
    //Add a particle to the particle system.
    if (x >= MAX_PARTICLES)
        return;
    //set postion of particle
    Particle *p = new Particle[MAX_PARTICLES];
    p->velocity.y = -0.2;
    p->velocity.x = ((double)rand() / (double)RAND_MAX) + 0.5;
    p->velocity.x = ((double)rand() / (double)RAND_MAX) + 0.5 + 0.10;
    x++;

    //Draw particles
    for (int i=0; i<y; i++) {
        glPushMatrix();
        glColor3f(1.0f,0.0f,0.0f); // red particle
        //Vec *c = particle[i].s.center;
        glEnd();
        glPopMatrix();
    }
}

void creditsKevin(Rect r)
{
    ggprint8b(&r, 16, 0x00004C00, "Kevin Mitra");
}

