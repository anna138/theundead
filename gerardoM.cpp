//author:Gerardo martinez
/*Four steps of Debugging
 *1)Recognize that a bug exists
 *2)Isolate the source of the bug
 *3)Identify the cause of the bug
 * 4)Determine a fix for the bug
 */
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


extern void postScores(char*);
void creditsGerardo(Rect r)
{
    ggprint8b(&r, 16, 0x00004C00, "Gerardo Martinez Flores");
}
void makeButton(int x, int y)
{
       // float theta;
        int width = x/2;
        int height = y/2;
  //      float angle=240;
        glPushMatrix();
        glBegin(GL_LINE_LOOP);
        // glBegin(GL_POLYGON);

      //  glBegin(GL_QUADS);
        for(int i=0;i<360;i++) {
           // theta =i*3.142/180;
           float rad = i*DEG2RAD;
           //glVertex2f(100*cos(theta)*width,100*sin(theta)*height);
            glVertex2f(cos(rad+200)*width,sin(rad)*height);
            glRotatef(100.0f,200.0,0,0.0f);
        }


       /* glVertex2i(width, -height);
        glVertex2i(width, height);
        glVertex2i(-width, height);
        glVertex2i(-width, -height);*/
        glEnd();
      // glFlush();
        glPopMatrix();

        //*(doneStart) = 1;

        //std::cout << *(doneStart) << std::endl;

}

void changeButtonColor( int y, int x , int &doneStart) {

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
bool posted = false;
void displaycurrentscore(Rect r, int h, int w, int bestScore,int yourScore){
    r.bot=h-h;
    r.left=w-w-25;
    r.center=0;
    int temp = rand();
    ggprint16(&r, 16, 0x003B8B68, "Game Over\n");
    ggprint16(&r, 16, 0x003B8B68, "Your score:%d \n", yourScore);
    ggprint16(&r, 16, 0x003B8B68, "Best score:%d \n", bestScore);
    ggprint16(&r, 16, 0x003B8B68, "\nTap to restart\n");
    char pn [100];
    
    sprintf(pn, "~mbal/3350/lab7/scores.php?name=Tom%d&score=%d", yourScore,temp); 
    if(!posted){
      posted = True;
      postScores(pn);
    }
}
