//Credits Screen
//

#include "fonts.h"
#include <GL/glx.h>
extern void creditsKevin(Rect);
extern void creditsAnna(Rect);
extern void creditsGerardo(Rect);
extern void creditManvir(Rect);

void showCredits()
{

    Rect r;

    r.bot = 0;
    r.left = 0;
    r.center = 0;
    creditManvir(r);
    ggprint8b(&r, 16, 0x00ffff00, "\n");
    creditsAnna(r);
    ggprint8b(&r, 16, 0x00ffff00, "\n");
    creditsGerardo(r);
    ggprint8b(&r, 16, 0x00ffff00, "\n");
    creditsKevin(r);
}

float t = -3.0f;
float inc = 0.005;
double xp = t; 
double yp = t;
void renderCoolCredits(){
    glClear(GL_COLOR_BUFFER_BIT);

	showCredits();
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
int angle = 240;	
    glOrtho(-4,4,-4,4,-1,1);
    //glFrustum(-4,4,-4,4,1.0,10);
    //glClear(GL_COLOR_BUFFER_BIT);
	//showCredits();
	glPushMatrix();
		//glVertex3d(xp,yp, -1.0);


        //showCredits(Rect r);
      //  glPopMatrix();//new info
		glEnable(GL_POINT_SMOOTH);
		//glColor3f(255,255,255);

		//glVertex2i(0,0);
		//float t = -3.0f;
		double xp = t; 
		double yp = t;
		double x = xp; 
		double y = yp;

		for (int i =  0; i < 1000;i++){
			// x = x_r[i];
			// y = y_r[i];
			xp = x*x - x*t + y + t;
			yp = x*x + y*y + t*t - x*t - x + y;
			x = xp;
			y = yp;
			//std::cout << "this is x: " << xp << " and this is yp:" << yp << std::endl;
			//sleep(1);
			if(i%1){
				glPointSize(5);
				//glColor3ub(rand()%255,rand()%255,rand()%255);
				glColor3ub(255,255,255);
			}
			else{
				glPointSize(5);
				glColor3ub(255,255,255);
			}
			glBegin(GL_POINTS);

			glVertex3d(xp,yp, -1.0);
			glEnd();

		}
	if(t >= 1.0f){
		inc = -inc;
	}else if(t <= -1.0f){
		inc = 0.005;
	}
	t += inc;

	glPopMatrix();


}
