GC := gcc
CFLAGS := -Wall -g -O2

all: out
	./out

out: main.o hardware.o
	${GC} ${CFLAGS} main.o hardware.o -o out
	
main.o: main.c hardware.h
	${GC} ${CFLAGS} main.c -c
	
hardware.o: hardware.c hardware.h
	${GC} ${CFLAGS} hardware.c -c	

clear:
	rm *.o
	rm out
