CC=gcc
CFLAGS=-ggdb -Wall -Wextra -Wpedantic
LL=-L raylib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

.PHONY: run all clean

all: facecraft

run: facecraft
	./facecraft

clean:
	rm -rf *.o
	# cd raylib/src && make clean # recursively clean raylib

facecraft: main.o blocks.o drawing.o raylib/libraylib.a
	$(CC) $(CFLAGS) -o $@ $^ $(LL)

raylib/libraylib.a:
	cd raylib/src && make PLATFORM=PLATFORM_DESKTOP # To make the static version.
	cp --update --verbose raylib/src/libraylib.a raylib/
	cp --update --verbose raylib/src/raylib.h raylib/
	cp --update --verbose raylib/src/rlgl.h raylib/

%.o: %.c *.h
	$(CC) -c $(CFLAGS) -o $@ $< $(LL)