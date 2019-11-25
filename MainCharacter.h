#pragma once 
#include "Texture.h"
#include "GlobalSpace.h"
class MainCharacter{

    private:
        int face;
        Texture *hitler;
		gvars::Direction dir;
    public:
        int pos[3];
		double mousepos[2];
		int angle;
        MainCharacter();
        ~MainCharacter();
        void characterRender();
        void setFace(int);
		void calFace();
};