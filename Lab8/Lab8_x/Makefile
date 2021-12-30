CC = gcc
CFLAGS = -g -Wall
OBJS = Lab8_x.o
TARGET = Lab8_x.out

all: $(TARGET)

$(TARGET): $(OBJS)
	        $(CC) -o $@ $(OBJS)

Lab8_x.o: Lab8_x.c


clean:
	rm -f *.o
	rm -f $(TARGET)
