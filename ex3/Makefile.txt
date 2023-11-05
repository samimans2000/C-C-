CC=gcc
CFLAGS= -Wall -Wextra -Wvla -std=c99

ex3: calculator.h calculator.c heat_eqn.c heat_eqn.h reader.c
	$(CC) $(CFLAGS) calculator.h calculator.c heat_eqn.c heat_eqn.h reader.c -o ex3

all: ex3
	./ex3 input.txt

clean: 
	-rm -f *.o ex3

