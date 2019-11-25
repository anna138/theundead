#pragma once
#include <GL/glx.h>
#include "Image.h"
class Texture{

    private:
        int w;
        int h;
        int x;
        int y;
        int z;
        GLuint id;
        Image *img;
    public:
        Texture();
        Texture(const char*, int, int, int, int, int);
        void set(const char*);
        void Display_Picture(int, int, int , int);
		unsigned char* buildAlphaData();
        int getID();
        ~Texture();
};
