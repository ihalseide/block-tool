CC=gcc
CFLAGS=-ggdb -Wall -Wextra -Wpedantic
LL=-L raylib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

.PHONY: run all clean raylib_stuff

all: facecraft

run: facecraft
	./facecraft

clean:
	rm -rf *.o
	# cd raylib/src && make clean # recursively clean raylib

facecraft: main.o blocks.o drawing.o cubeAtlas.o raylib/libraylib.a
	$(CC) $(CFLAGS) -o $@ $^ $(LL)

raylib_stuff: raylib/libraylib.a raylib/rlgl.h raylib/raylib.h raylib/raymath.h

raylib/%.h: raylib/src/%.h
	cp --update --verbose raylib/src/%.h raylib/

raylib/libraylib.a:
	cd raylib/src && make PLATFORM=PLATFORM_DESKTOP # To make the static version.

%.o: %.c *.h
	$(CC) -c $(CFLAGS) -o $@ $< $(LL)