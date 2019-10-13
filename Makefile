CFLAGS = -I ./include -std=c++11
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm -lssl -lcrypto #-lXrandr

all: asteroids

asteroids: manvirB.cpp gerardoM.cpp kevinM.cpp annaP.cpp credits.cpp asteroids.cpp log.cpp timers.cpp lab3sget.cpp Image.o Trooper.o Villain.o Bullet.o Global.o Highscores.o
	g++ $(CFLAGS) asteroids.cpp Global.o manvirB.cpp gerardoM.cpp annaP.cpp kevinM.cpp credits.cpp log.cpp timers.cpp lab3sget.cpp Image.o Trooper.o Villain.o Bullet.o Highscores.o libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids 

Global.o: Global.cpp
	g++ -c Global.cpp 

Image.o: Image.cpp
	g++ -c Image.cpp 
	
Highscores.o: Highscores.cpp
	g++ -c Highscores.cpp 

Trooper.o: Trooper.cpp
	g++ -c Trooper.cpp 
	
Villain.o: Villain.cpp
	g++ -c Villain.cpp 

Bullet.o: Bullet.cpp
	g++ -c Bullet.cpp 
	
clean:
	rm -f asteroids
	rm -f *.o

