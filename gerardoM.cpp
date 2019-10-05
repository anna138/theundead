//author:Gerardo martinez

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include <math.h>
#define DEG2RAD 3.14159/180.0
void creditsGerardo(Rect r)
{
    ggprint8b(&r, 16, 0x00004C00, "Gerardo Martinez Flores");
}
void makeButton(Rect r, int y, int x, int img_x , int img_y)
{
        float theta;
        int width = x/2;
        int height = y/2;
        glPushMatrix();
      //glBegin(GL_LINE_LOOP);
         glBegin(GL_POLYGON);

      //  glBegin(GL_QUADS);
        for(int i=0;i<360;i++) {
           //float rad = i*DEG2RAD;
            theta =i*3.142/180;
            glVertex2f(100*cos(theta),100*sin(theta));
           // glVertex2f(cos(rad)*width,sin(rad)*height);

        }


       /* glVertex2i(width, -height);
        glVertex2i(width, height);
        glVertex2i(-width, height);
        glVertex2i(-width, -height);*/
        glEnd();
       glFlush();
        glPopMatrix();

        //*(doneStart) = 1;

        //std::cout << *(doneStart) << std::endl;

}

void changeButtonColor(Rect r, int y, int x , int img_x, int img_y, int &doneStart) {

            int width = x/2;
            int height = y/2;
            glPushMatrix();
            glColor3f(0.0,1.0,1.0);
            glBegin(GL_QUADS);
            glVertex2i(width, -height);
            glVertex2i(width, height);
            glVertex2i(-width, height);
            glVertex2i(-width, -height);
            glEnd();
            glPopMatrix();
        doneStart = 0;
}
