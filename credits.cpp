//Credits Screen
//

#include "fonts.h"
#include <GL/glx.h>
#include <ctime>
#include <cstdlib>

extern void chaos_equations();
extern void displayBackground(int w, int h, unsigned int imageTexture);
extern void creditsKevin(Rect);
extern void creditsAnna(Rect);
extern void creditsGerardo(Rect);
extern void creditManvir(Rect);

int move = 0;

void showCredits(int w, int h)
{

    Rect r;
    r.bot = h/2-25;
    r.left = -w/2+25+move;
    r.center = 0;
    creditManvir(r);
    ggprint8b(&r, 16, 0x00004C00, "\n");
    creditsAnna(r);
    ggprint8b(&r, 16, 0x00004C00, "\n");
    creditsGerardo(r);
    ggprint8b(&r, 16, 0x00004C00, "\n");
    creditsKevin(r);
    ggprint8b(&r, 16, 0x00ffffff, "\n");

    move = (move + 1) % w;
}

void renderCoolCredits(int w, int h, unsigned int imageTexture){
    glClear(GL_COLOR_BUFFER_BIT);
	displayBackground(w, h,imageTexture);
	showCredits(w, h);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glFrustum(-w/2,w/2,-h/2,h/2, 1.0,3);
    chaos_equations();

}
