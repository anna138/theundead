//Credits Screen
//

#include "fonts.h"
#include <GL/glx.h>
extern void chaos_equations();

void showCredits(Rect r)
{

    ggprint8b(&r, 16, 0x00ff0000, "Credits of this Game is By:");
    ggprint8b(&r, 16, 0x00ff0000, "Kevin Mitra");
    ggprint8b(&r, 16, 0x00ff0000, "Gerardo Martinez");
    ggprint8b(&r, 16, 0x00ff0000, "Manvir Bal");
    ggprint8b(&r, 16, 0x00ff0000, "Anna Poon");
}

void renderCoolCredits(){
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //glOrtho(-4,4,-4,4,-1,1);
    glFrustum(-4,4,-4,4,1.0,10);


    chaos_equations();


}