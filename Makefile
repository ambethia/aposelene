CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-framework Cocoa -framework OpenGL -lglfw
ENGINE= engine/common.c engine/font.c engine/utility.c

all: demo

demo: $(ENGINE)

clean:
	rm -rf engine/*.o *.o demo *.dSYM
