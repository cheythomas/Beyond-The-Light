CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: game

game: game.cpp ppm.cpp log.cpp
	g++ $(CFLAGS) game.cpp ppm.cpp log.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -owalk

auroraH.o: auroraH.cpp game.h
	g++ $(FLAGS) -wall -Wextra -c auroraH.cpp -o auroraH.o
	
cheyenneT.o: cheyenneT.cpp game.h
	g++ $(CFLAGS) -Wall -Wextra -c cheyenneT.cpp -o cheyenneT.o

KarenS.o: karenS.cpp game.h
	g++ $(CFLAGS) -Wall -Wextra -c karenS.cpp -o karenS.o
	
ppm.o: ppm.cpp
	g++ $(CFLAGS) -Wall -Wextra -c ppm.cpp -o ppm.o

log.o: log.cpp
	g++ $(CFLAGS) -Wall -Wextra -c log.cpp -o log.o	
	
btl: game.o auroraH.o cheyenneT.o karenS.o game.h
	g++ $(CFLAGS) $(LFLAGS) ppm.o log.o walk.o auroraH.o cheyenneT.o karenS.o libggfonts.a -Wall -Wextra -o btl


clean:
	rm -f btl
	rm -f *.o

