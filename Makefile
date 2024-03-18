CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic
LL=-L raylib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

.PHONY: run all clean

run: facecraft
	./facecraft

all: facecraft

clean:
	rm -rf *.o
	cd raylib/src && make clean # recursively clean raylib

facecraft: main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LL)

raylib/libraylib.a:
	cd raylib/src && make PLATFORM=PLATFORM_DESKTOP # To make the static version.
	cp raylib/src/libraylib.a raylib/
	cp raylib/src/raylib.h raylib/

%.o: %.c raylib/libraylib.a
	$(CC) -c $(CFLAGS) -o $@ $< $(LL)