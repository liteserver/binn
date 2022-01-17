
CFLAGS=-O0 -g0 -Wall -std=gnu11 -I../../src

all:
	gcc $(CFLAGS) -c ../../src/binn.c -o binn.o
	gcc $(CFLAGS) -c static.c -o static.o
	gcc $(CFLAGS) -o static static.o binn.o

clean:
	-rm binn.o
	-rm static.o
