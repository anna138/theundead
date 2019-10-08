CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: asteroids

asteroids: annaP.cpp manvirB.cpp gerardoM.cpp kevinM.cpp credits.cpp asteroids.cpp log.cpp timers.cpp Image.o Trooper.o Villain.o Bullet.o Global.o
	g++ $(CFLAGS) asteroids.cpp Global.o annaP.cpp manvirB.cpp gerardoM.cpp kevinM.cpp credits.cpp log.cpp timers.cpp Image.o Trooper.o Villain.o Bullet.o  libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids 

Global.o: Global.cpp
	g++ -c Global.cpp 

Image.o: Image.cpp
	g++ -c Image.cpp 
	
Trooper.o: Trooper.cpp
	g++ -c Trooper.cpp 
	
Villain.o: Villain.cpp
	g++ -c Villain.cpp 

Bullet.o: Bullet.cpp
	g++ -c Bullet.cpp 
	
clean:
	rm -f asteroids
	rm -f *.o

