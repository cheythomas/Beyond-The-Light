CFLAGS = -I ./include -g
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
#-D USE_OPENAL_SOUND \
#/usr/lib/x86_64-linux-gnu/libopenal.so \
#/usr/lib/x86_64-linux-gnu/libalut.so

all: game

game:	game.cpp auroraH.cpp cheyenneT.cpp karenS.cpp ppm.cpp 
	g++ -D ENABLE_AUDIO $(CFLAGS) $(LFLAGS) game.cpp auroraH.cpp cheyenneT.cpp karenS.cpp fonts.cpp ppm.cpp log.cpp libggfonts.a -Wall -Wextra  $(LFLAGS) -ogame \
	-D USE_OPENAL_SOUND \
  	/usr/lib/x86_64-linux-gnu/libopenal.so \
  	/usr/lib/x86_64-linux-gnu/libalut.so


clean:
	rm -f game 

