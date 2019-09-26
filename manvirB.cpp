//Author: Manvir Bal
//created on: 9/17/2019
//
#include "fonts.h"
#include <GL/glx.h>
#include <iostream>
#include <unistd.h>

struct Vec
{
	float x, y, z;
	Vec(){ z = (0), y = (0), x = (0);}
	Vec(float x1, float y1, float z1) : x(x1), y(y1), z(z1) {}
	void setPoints(float x1, float y1, float z1)
	{
		x = x1;
		y = y1;
		z = z1;
	}
};
const int MAX_PARTICLES = 10000;
Vec particles[MAX_PARTICLES];

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
void makeParticles(int w, int h)
{
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		particles[i].x = ((float)(rand() % w)) - (w / 2);
		particles[i].y = ((float)(rand() % h)) - (h / 2);
		particles[i].z = ((float)rand() / RAND_MAX) * -100;
		//printf("z: %f\n", particles[i].z);
	}
}

float t = 0.05f;
float inc = 0.00005;
double xp = t; 
double yp = t;
int angle = 5;
float z = 0.005;
void chaos_equations(){
	//glClear(GL_COLOR_BUFFER_BIT);
	//draw dots
	glTranslatef(0,0,z);
	glPushMatrix();
		glPointSize(3);
		
		glBegin(GL_POINTS);
			for(int i = 0; i < MAX_PARTICLES; i++){
				glVertex3f(particles[i].x, particles[i].y, particles[i].z);
			}
		glEnd();
	glPopMatrix();

	glPushMatrix();
		//glRotatef(angle, 0,0,1);
		glScalef(1000,500,1);
		glTranslatef(0,0,-(z+1));
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
				glVertex3d(xp,yp,-1.1);
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
	if (z > 50)
		z = 0;
	z += 0.005;
	
}
