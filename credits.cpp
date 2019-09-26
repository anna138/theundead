//Credits Screen
//

#include "fonts.h"
#include <GL/glx.h>
#include <ctime>
#include <cstdlib>

extern void chaos_equations();
extern void displayLogo(int w, int h, int img_x, int img_y, GLuint imageTexture);
extern void creditsKevin(Rect);
extern void creditsAnna(Rect);
extern void creditsGerardo(Rect);
extern void creditManvir(Rect);


void showCredits(int w, int h)
{

    Rect r;
    r.bot = h/2-12;
    r.left = -w/2+10;
    r.center = 0;
    creditManvir(r);
    ggprint8b(&r, 16, 0x00ffff00, "\n");
    creditsAnna(r);
    ggprint8b(&r, 16, 0x00ffff00, "\n");
    creditsGerardo(r);
    ggprint8b(&r, 16, 0x00ffff00, "\n");
    creditsKevin(r);
    ggprint8b(&r, 16, 0x00ffff00, "\n");

    
}

void renderCoolCredits(int w, int h, int img_x, int img_y, GLuint imageTexture){
    glClear(GL_COLOR_BUFFER_BIT);
	showCredits(w, h);
	displayLogo(w, h, img_x, img_y, imageTexture);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    // glOrtho(-4,4,-4,4,-1,1);
	glFrustum(-w/2,w/2,-h/2,h/2, 1.0,5);
    chaos_equations();

}
