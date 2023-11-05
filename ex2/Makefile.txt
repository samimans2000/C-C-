CC=gcc
CFLAGS= -Wall -Wextra -Wvla -std=c99

all: battleships battleships_game
	$(CC) battleships.o battleships_game.o -o ex2.exe

battleships: battleships.c battleships.h
	$(CC) $(CFLAGS) -c battleships.c

battleships_game: battleships.c battleships.h battleships_game.c
	$(CC) $(CFLAGS) -c battleships.c battleships_game.c

clean: 
	-rm -f *.o ex2.exe
