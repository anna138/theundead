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
	Skull *skull;
	// Skull *skulls = new Skull[gvars::MAX_SKULLS];
	int level = 1;
	Zombie *zombie;
	
	int zombiecount = level*3; 
	int skullcount = level*3; 
	Asteroid *asteroid;
	Bullet *barr;
	int nasteroids;
	int nbullets;
	struct timespec bulletTimer;
	struct timespec mouseThrustTimer;
	bool mouseThrustOn;
public:
	Game() {
		hero.heal(600);
		barr = new Bullet[MAX_BULLETS];
		zombie = new Zombie[zombiecount];
		skull = new Skull[skullcount];
		asteroid = NULL;
		nasteroids = 0;
		nbullets = 0;

		mouseThrustOn = false;

		clock_gettime(CLOCK_REALTIME, &bulletTimer);
	}
	void reset(){
		delete [] zombie;
		zombie = new Zombie[zombiecount];
	}
	~Game() {
		delete [] skull;
		delete [] zombie;
		delete [] barr;
	}
} g;