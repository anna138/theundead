CFLAGS = -I ./include -g 
ERRORFLAGS = -Wall #-Werror -Wextra
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm -lssl -lcrypto #-lXrandr

all: undead undeadSound

GlobalSpace.o: GlobalSpace.cpp
	g++ -c GlobalSpace.cpp

TileParser.o: ./Tiled/TileParser.cpp
	g++ -c ./Tiled/TileParser.cpp

pugixml.o: ./Tiled/pugixml.cpp
	g++ -c ./Tiled/pugixml.cpp

undeadSound: GlobalSpace.o pugixml.o TileParser.o manvirB.cpp gerardoM.cpp kevinM.cpp annaP.cpp credits.cpp undead.cpp log.cpp timers.cpp Image.o Trooper.o Zombie.o Skull.o Bullet.o Global.o Highscores.o 
	g++ $(CFLAGS) undead.cpp Global.o pugixml.o TileParser.o GlobalSpace.o manvirB.cpp gerardoM.cpp annaP.cpp kevinM.cpp credits.cpp log.cpp timers.cpp Image.o Trooper.o Zombie.o Skull.o Bullet.o Highscores.o libggfonts.a $(ERRORFLAGS) $(LFLAGS) -oundeadSound \
	-D USE_OPENAL_SOUND \
	/usr/lib/libopenal.so \
	/usr/lib/libalut.so 

undead: GlobalSpace.o pugixml.o TileParser.o manvirB.cpp gerardoM.cpp kevinM.cpp annaP.cpp credits.cpp undead.cpp log.cpp timers.cpp Image.o Trooper.o Zombie.o Skull.o Bullet.o Global.o Highscores.o 
	g++ $(CFLAGS) undead.cpp Global.o pugixml.o TileParser.o GlobalSpace.o manvirB.cpp gerardoM.cpp annaP.cpp kevinM.cpp credits.cpp log.cpp timers.cpp Image.o Trooper.o Zombie.o Skull.o Bullet.o Highscores.o libggfonts.a $(ERRORFLAGS) $(LFLAGS) -oundead

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
	
clean:
	rm -f undead
	rm -f *.o
	rm -f undeadSound
