CFLAGS = -I ./include -g
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: btl

btl: log.o ppm.o game.o auroraH.o cheyenneT.o karenS.o
	g++ $(CFLAGS) $(LFLAGS) ppm.o log.o game.o auroraH.o cheyenneT.o karenS.o libggfonts.a -Wall -Wextra -o btl

game.o: game.cpp
	g++ $(CFLAGS) -Wall -Wextra -c game.cpp -o game.o

auroraH.o: auroraH.cpp game.h
	g++ $(CFLAGS) -Wall -Wextra -c auroraH.cpp -o auroraH.o

cheyenneT.o: cheyenneT.cpp game.h
	g++ $(CFLAGS) -Wall -Wextra -c cheyenneT.cpp -o cheyenneT.o

KarenS.o: karenS.cpp game.h
	g++ $(CFLAGS) -Wall -Wextra -c karenS.cpp -o karenS.o

ppm.o: ppm.cpp
	g++ $(CFLAGS) -Wall -Wextra -c ppm.cpp -o ppm.o

log.o: log.cpp
	g++ $(CFLAGS) -Wall -Wextra -c log.cpp -o log.o


clean:
	rm -f btl *.o

