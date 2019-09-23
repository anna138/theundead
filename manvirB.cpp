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
float t = -3.0f;
float inc = 0.1;
double xp = t; 
double yp = t;
double x_rand[100];
double y_rand[100];
void popRand(){
	for(int i = 0; i < 100; i++){
		x_rand[i] = (2*((float)(rand()%20)/20)-1)*20;
		y_rand[i] = (2*((float)(rand()%20)/20)-1)*20;
	}
}

void renderCoolCredits(){
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
		glEnable(GL_POINT_SMOOTH);
		//glPointSize(4);
		//glColor3f(255,255,255);
		
		//glVertex2i(0,0);
		//float t = -3.0f;
		double xp = t; 
		double yp = t;
		double x = xp; 
		double y = xp;

		for (int i = 0; i < 100;i++){
			//x = x_rand[i];
			//y = y_rand[i];
			xp = x*x - x*t + y + t;
			yp = x*x + y*y + t*t - x*t - x + y;
			
			//std::cout << "this is xp" << xp << " and this is yp" << yp << std::endl;
			//sleep(1);
			if(i%3){
				glPointSize(5);
				//glColor3ub(rand()%255,rand()%255,rand()%255);
				glColor3ub(255,255,255);
			}
			else{
				glPointSize(1);
				glColor3ub(255,255,255);
			}
			glBegin(GL_POINTS);
			
			glVertex2d(xp,yp);
			glEnd();
		}
		
	glPopMatrix();

	if(t >= 3.0f){
		inc = -inc;
	}else if(t <= -3.0f){
		inc = 0.1;
	}
	t += inc;
	
}

