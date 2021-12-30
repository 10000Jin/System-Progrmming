all: tug_of_war.out view.out
CC = gcc
CFLAGS = -g -Wall

tug_of_war.out: tug_of_war.c
	$(CC) -o $@ $^

view.out: view.c
	$(CC) -o $@ $^


clean:
	rm -f *.out
