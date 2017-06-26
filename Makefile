CFLAGS = -I ./include -g
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: game

game:	game.cpp auroraH.cpp cheyenneT.cpp karenS.cpp ppm.cpp 
	g++ $(CFLAGS) $(LFLAGS) game.cpp auroraH.cpp cheyenneT.cpp karenS.cpp ppm.cpp log.cpp libggfonts.a -Wall -Wextra  $(LFLAGS) -ogame



clean:
	rm -f game 

