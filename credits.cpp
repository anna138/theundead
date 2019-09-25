//Credits Screen
//

#include "fonts.h"
#include <GL/glx.h>
#include <ctime>
#include <cstdlib>

extern void chaos_equations();
extern void creditsKevin(Rect);
extern void creditsAnna(Rect);
extern void creditsGerardo(Rect);
extern void creditManvir(Rect);

void showCredits()
{

    Rect r;
    r.bot = 400;
    r.left = -500;
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

void renderCoolCredits(){
    glClear(GL_COLOR_BUFFER_BIT);
	showCredits();
	//glMatrixMode(GL_PROJECTION); glLoadIdentity();
	//glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //glOrtho(-4,4,-4,4,-1,1);
    //glFrustum(-4,4,-4,4,1.0,10);
    chaos_equations();

    
}
