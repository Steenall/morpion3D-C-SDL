board: game.o board.o
	gcc game.o board.o `sdl2-config --libs --cflags` -Wall -lSDL2_image -lm -o game

game.o: board.h
	gcc game.c `sdl2-config --libs --cflags` -Wall -lSDL2_image -lm -c -o game.o

board.o:
	gcc board.c -Wall -c -o board.o

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper:
	rm -rf game
