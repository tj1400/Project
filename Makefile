CFLAGS = -I ./include terryL.cpp maximillianW.cpp andrewB.cpp #guangL.cpp
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: walk

walk: walk.cpp
	g++ walk.cpp $(CFLAGS) libggfonts.a -Wall -Wextra $(LFLAGS) -owalk

clean:
	rm -f walk
	rm -f *.o

