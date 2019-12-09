#pragma once 
#include "Texture.h"
#include "GlobalSpace.h"

class MainCharacter{

    private:
        Texture *trooper;

    public:
    	enum class Direction{
			S,SW,W,NW,N,NE,E,SE,end
    	};
        int face;
		Direction dir;
        int pos[3];
        int tile;
        int lifeForce; // Life force --> Hud Level
		double mousepos[2];
        MainCharacter();
        ~MainCharacter();
        void characterRender();
        void setFace(int);
		void calFace();
        bool isCollide(int *, int &, int); /* anna.cpp*/  
        void heal(int); /* anna.cpp - heals character;*/
        void damage(int); /* anna.cpp - damage to character;*/
        void recovery(int);
        void reset();
};