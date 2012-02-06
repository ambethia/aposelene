CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-framework Cocoa -framework OpenGL -lglfw

all: demo

clean:
	rm -rf *.o demo
