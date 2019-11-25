#pragma once 
#include "Texture.h"

class MainCharacter{

    private:
        int dir;
        int face;
        Texture *hitler;
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