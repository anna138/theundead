#pragma once
#include <GL/glx.h>


class Global {
public:
	int xres, yres;
	char keys[65536];
	GLuint backgroundTexture;
	GLuint startTexture;
	GLuint trooperTexture;
	GLuint villainTexture;
	Global();
};
