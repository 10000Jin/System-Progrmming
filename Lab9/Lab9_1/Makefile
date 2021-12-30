all: Lab9_1.out sample.out
CC = gcc
CFLAGS = -g -Wall

Lab9_1.out: Lab9_1.c
	$(CC) -o $@ $^

sample.out: sample.c
	$(CC) -o $@ $^


clean:
	rm -f *.out
