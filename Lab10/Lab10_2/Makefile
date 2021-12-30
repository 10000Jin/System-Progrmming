all: frontman.out participant.out
CC = gcc
CFLAGS = -g -Wall

frontman.out: frontman.c
	$(CC) -o $@ $^

participant.out: participant.c
	$(CC) -o $@ $^


clean:
	rm -f *.out
