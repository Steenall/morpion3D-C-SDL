game : board.o game.o
	gcc .\bin\game.o .\bin\board.o -o .\bin\prog -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

game.o: .\src\board.h
	gcc .\src\game.c -c -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o .\bin\game.o

board.o:
	gcc .\src\board.c -Wall -c -o .\bin\board.o

.PHONY: clean mrproper

clean:
	rm -rf .\bin\*.o

mrproper:
	rm -rf game
