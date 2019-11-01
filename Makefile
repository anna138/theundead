CFLAGS = -I ./include -g
ERRORFLAGS = -Wall -Werror -Wextra
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm -lssl -lcrypto #-lXrandr

all: asteroids

asteroids: manvirB.cpp gerardoM.cpp kevinM.cpp annaP.cpp credits.cpp asteroids.cpp log.cpp timers.cpp Image.o Trooper.o Zombie.o Skull.o Bullet.o Global.o Highscores.o GlobalSpace.o
	g++ $(CFLAGS) asteroids.cpp Global.o manvirB.cpp gerardoM.cpp annaP.cpp kevinM.cpp credits.cpp log.cpp timers.cpp Image.o Trooper.o Zombie.o Skull.o Bullet.o Highscores.o GlobalSpace.o libggfonts.a $(ERRORFLAGS) $(LFLAGS) -oasteroids 

Global.o: Global.cpp
	g++ -c Global.cpp 

Image.o: Image.cpp
	g++ -c Image.cpp 
	
Highscores.o: Highscores.cpp
	g++ -c Highscores.cpp 

Trooper.o: Trooper.cpp
	g++ -c Trooper.cpp 
	
Skull.o: Skull.cpp
	g++ -c Skull.cpp 

Zombie.o: Zombie.cpp
	g++ -c Zombie.cpp 

Bullet.o: Bullet.cpp
	g++ -c Bullet.cpp 
	
GlobalSpace.o: GlobalSpace.cpp
	g++ -c GlobalSpace.cpp

clean:
	rm -f asteroids
	rm -f *.o

