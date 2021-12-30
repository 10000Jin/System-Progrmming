all: sender.out receiver.out
CC = gcc
CFLAGS = -g -Wall

sender.out: sender.c
	$(CC) -o $@ $^

receiver.out: receiver.c
	$(CC) -o $@ $^


clean:
	rm -f *.out
