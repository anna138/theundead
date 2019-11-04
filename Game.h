#pragma once
#include "Asteroid.h"
#include "Bullet.h"
#include "Skull.h"
#include "Zombie.h"
#include "Trooper.h"
#include "Macros.h"
#include <ctime>
#include "GlobalSpace.h"

using namespace gvars;

class Game {
public:
	Trooper trooper;
	Skull skull;
	Zombie zombie;
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