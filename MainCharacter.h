#pragma once 
#include "Texture.h"
#include "GlobalSpace.h"

class MainCharacter{

    private:
        int face;
        //Texture *hitler;
        Texture *trooper;

    public:
    	enum class Direction{
			S,SW,W,NW,N,NE,E,SE,end
    	};
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
        void heal(int &, int); /* anna.cpp - heals character;*/
        void damage(int &, int); /* anna.cpp - damage to character;*/
        void recovery(int &, int);
};