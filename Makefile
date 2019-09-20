CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: asteroids

asteroids: annaP.cpp manvirB.cpp gerardoM.cpp kevinM.cpp asteroids.cpp log.cpp timers.cpp
	g++ $(CFLAGS) asteroids.cpp annaP.cpp manvirB.cpp gerardoM.cpp kevinM.cpp log.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids

clean:
	rm -f asteroids
	rm -f *.o

