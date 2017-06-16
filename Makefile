#
CFLAGS = -I ./include
##LIB    = ./lib/fmod/libfmodex64.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: rainforest

rainforest: rainforest.cpp ppm.cpp log.cpp cheyenneT.cpp auroraH.cpp karenS.cpp
	g++ $(CFLAGS) rainforest.cpp cheyenneT.cpp ppm.cpp log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o rainforest

clean:
	rm -f rainforest
	rm -f *.o

