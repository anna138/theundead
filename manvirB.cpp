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
int colorArray[100];
void randomColor(){
	for(int i = 0; i < 100; i++){
		colorArray[i] = rand()%255;
	}
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
			glPointSize(5);
			glColor3ub(colorArray[i%100], colorArray[(i+1)%100], colorArray[(i+2)%100]);
			glBegin(GL_POINTS);
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
