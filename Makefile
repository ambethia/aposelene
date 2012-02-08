CC=gcc
CFLAGS=-Wall -g -IEngine
LDFLAGS=-framework Cocoa -framework OpenGL -lglfw
ENGINE= Engine/common.c Engine/font.c Engine/utility.c

all: demo

demo: $(ENGINE)

clean:
	rm -rf engine/*.o *.o demo *.dSYM
