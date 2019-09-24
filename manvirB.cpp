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
double x_r[1000];
double y_r[1000];
void genRand(){
	for(int i = 1; i < 1000; i++){
		float t = (4*((float)rand()/RAND_MAX))-2;
		float x = (4*((float)rand()/RAND_MAX))-2;
		float y = (4*((float)rand()/RAND_MAX))-2;
		x_r[i] = x_r[i-1] + x*x - x*t + y + t;
		y_r[i] = y_r[i-1] + x*x + y*y + t*t - x*t - x + y;
	}
}
float t = -3.0f;
float inc = 0.005;
double xp = t; 
double yp = t;
void renderCoolCredits(){
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(-4,4,-4,4,-1,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
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

			glVertex2d(xp,yp);
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