#pragma once
#include "Asteroid.h"
#include "Bullet.h"
#include "Skull.h"
#include "Zombie.h"
#include "Trooper.h"
#include "Macros.h"
#include <ctime>
#include "GlobalSpace.h"
#include <iostream>

using namespace gvars;

/*Extern Prototypes*/ 
extern void movingImages(int width_x, int height_y, Vec img_pos, float img_angle, unsigned int texture);

class Game {
public:
	Trooper trooper;
	Skull skull;
	Skull *skulls = new Skull[gvars::MAX_SKULLS];
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
		hero.heal(60000);
		barr = new Bullet[MAX_BULLETS];

		asteroid = NULL;
		nasteroids = 0;
		nbullets = 0;

		mouseThrustOn = false;

		clock_gettime(CLOCK_REALTIME, &bulletTimer);
	}
	~Game() {
		delete [] barr;
	}
} g;