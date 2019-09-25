//Author: Manvir Bal
//created on: 9/17/2019
//
#include "fonts.h"
#include <GL/glx.h>
#include <iostream>
#include <unistd.h>



void creditManvir(Rect r)
{
    ggprint8b(&r, 16, 0x00ff0000, "Manvir Bal");
}
float t = 0.05f;
float inc = 0.00005;
double xp = t; 
double yp = t;
int angle = 5;
void chaos_equations(){
	//glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
		//glRotatef(angle, 0,0,1);
		glScalef(1000,500,1);
		glEnable(GL_POINT_SMOOTH);
		xp = t;
		yp = t;
		double x = xp; 
		double y = yp;

		for (int i =  0; i < 1000;i++){
			xp = x*x - x*t + y + t;
			yp = x*x + y*y + t*t - x*t - x + y;
			x = xp;
			y = yp;
			// std::cout << "this is x: " << xp << " and this is yp:" << yp << std::endl;
			// sleep(1);

			glPointSize(5);
			glColor3ub(rand()%255,rand()%255,rand()%255);
			//glColor3ub(255,255,255);
			glBegin(GL_POINTS);
				//glVertex2d(xp,yp);
				glVertex3d(xp,yp, 0);
			glEnd();

		}
	if(t >= 0.05f){
		inc = -inc;
	}else if(t <= -0.05f){
		inc = 0.00005;
	}
	t += inc;
	angle = (angle + 1)%360;

	glPopMatrix();
}
