
CC = gcc -o
CFLAGS = -g -Wall

default: shell
main: list.h
	$(CC) $(CFLAGS)  shell list.h list.c
clean:
	-rm *~
