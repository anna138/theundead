// Modified by: Kevin Mitra
// CMPS 3350
// Modified Date: 10/11/2019

/* Steps for Debugging code
 * 1. Reproduce the bug before changing code
 * 2. Understand stack traces
 * 3. Write a test case that Reproduces the bug
 * 4. Know your error codes
 */

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/glut.h>
#include "fonts.h"
#include <math.h>

using namespace std;

void makeParticle(int a, int b);
void creditsKevin(Rect r);

struct Vec 
{
    float x, y, z;
}; 

const int MAX_PARTICLES = 15000;

struct Shape 
{
    float width = 80, height = 10;
    float radius;
    Vec center;
};

struct Particle 
{
    Shape s;
    Vec velocity;
};

class MusicT 
{

};

void makeParticle(int x, int y)
{
    //Add a particle to the particle system.
    if (x >= MAX_PARTICLES)
        return;
    //set postion of particle
    Particle *p = new Particle[MAX_PARTICLES];
    p->velocity.y = -0.2;
    p->velocity.x = ((double)rand() / (double)RAND_MAX) + 0.5;
    p->velocity.x = ((double)rand() / (double)RAND_MAX) + 0.5 + 0.10;
    x++;

    //Draw particles
    for (int i=0; i<y; i++) {
        glPushMatrix();
        glColor3f(1.0f,0.0f,0.0f); // red particle
        //Vec *c = particle[i].s.center;
        glEnd();
        glPopMatrix();
    }
}

void creditsKevin(Rect r)
{
    ggprint8b(&r, 16, 0x00004C00, "Kevin Mitra");
}

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#ifdef USE_OPENAL_SOUND

#include </usr/include/AL/alut.h>

#include <ctime>
#endif //USE_OPENAL_SOUND

int stop = 0;

int music(std::vector<std::string> files)
{
	//Get started right here.
#ifdef USE_OPENAL_SOUND
	alutInit(NULL, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		return 0;
	}
	//Clear error state.
	alGetError();
	//
	//Setup the listener.
	//Forward and up vectors are used.
	//The person listening is facing forward toward the sound.
	//The first 3 components of vec are 0,0,1
	//this means that the person is facing x=0, y=0, z=1, forward.
	//The second 3 components means that up is x=0,y=1,z=0, straight up!
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Buffers hold the sound information.
	ALuint alBuffer[1];
    int rnd = rand()%files.size();
	alBuffer[0] = alutCreateBufferFromFile(files.at(rnd).c_str());
	//let's create a looping sound.
	//alBuffer[1] = alutCreateBufferFromFile("./Credits1.wav");

	//
	//Source refers to the sound.
	ALuint alSource[1];
	//Generate 2 sources, and store in the matching buffers.
	alGenSources(1, alSource);
	alSourcei(alSource[0], AL_BUFFER, alBuffer[0]);
	//alSourcei(alSource[1], AL_BUFFER, alBuffer[1]);
	//
	//FirstSet volume and pitch to normal, no looping of sound.
	// alSourcef(alSource[0], AL_GAIN, 1.0f);
	// alSourcef(alSource[0], AL_PITCH, 1.0f);
	// alSourcei(alSource[0], AL_LOOPING, AL_FALSE);
	// if (alGetError() != AL_NO_ERROR) {
	// 	printf("ERROR: setting source\n");
	// 	return 0;
	// }
	//now a looping sound... jet engine.
	alSourcef(alSource[0], AL_GAIN, 0.5f);
	alSourcef(alSource[0], AL_PITCH, 1.0f);
	alSourcei(alSource[0], AL_LOOPING, AL_TRUE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return 0;
	}
	alSourcePlay(alSource[0]);
    stop = 0;
	for (int i=0; i<42; i++) {
        
		alSourcePlay(alSource[0]);
        for(int j = 1000000; j <= 48000000; j+=1000000){
            if(stop){break;}
            usleep(j);
        }
        if(stop){	
            alDeleteSources(1, &alSource[0]);
            //alDeleteSources(1, &alSource[1]);
            //Delete the buffers.
            alDeleteBuffers(1, &alBuffer[0]);

            return 0;
            break;
            
        }

	}
	//Cleanup.
	//First delete the sources.
	alDeleteSources(1, &alSource[0]);
	//alDeleteSources(1, &alSource[1]);
	//Delete the buffers.
	alDeleteBuffers(1, &alBuffer[0]);
	//alDeleteBuffers(1, &alBuffer[1]);
	//
	//Close out OpenAL itself.
	//Get active context.
	// ALCcontext *Context = alcGetCurrentContext();
	// //Get device for active context.
	// ALCdevice *Device = alcGetContextsDevice(Context);
	// //Disable context.
	// alcMakeContextCurrent(NULL);
	// //Release context(s).
	// alcDestroyContext(Context);
	// //Close device.
	// alcCloseDevice(Device);
#endif //USE_OPENAL_SOUND
	return 0;
}
void closemusic()
{
#ifdef USE_OPENAL_SOUND

	ALCcontext *Context = alcGetCurrentContext();
	//Get device for active context.
	ALCdevice *Device = alcGetContextsDevice(Context);
	//Disable context.
	alcMakeContextCurrent(NULL);
	//Release context(s).
	alcDestroyContext(Context);
	//Close device.
	alcCloseDevice(Device);

#endif
}