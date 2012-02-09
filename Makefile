CC=gcc
CFLAGS=-Wall -g -IEngine -IResources
LDFLAGS=-framework Cocoa -framework OpenGL -lglfw
ENGINE= Engine/*.c

all: demo

demo: $(ENGINE)

clean:
	rm -rf engine/*.o *.o demo *.dSYM
