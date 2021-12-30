all: jiknyeo.out kyunu.out
CC = gcc
CFLAGS = -g -Wall

jiknyeo.out: jiknyeo.c
	$(CC) -o $@ $^

kyunu.out: kyunu.c
	$(CC) -o $@ $^


clean:
	rm -f *.out
