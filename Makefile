CC=clang.exe 
CFLAGS=-Wall -Werror -Wno-pragma-pack
LDFLAGS=-lSDL2 -L. 
BINARY=aiv_renderer

ifeq ($(OS),Windows_NT)
	BINARY:=$(BINARY).exe
	BINARY_TESTS:=$(BINARY_TESTS).exe
endif

aiv_renderer: main.o aiv_rasterizer.o aiv_math.o
	$(CC) -o $(BINARY) $(LDFLAGS) $^

main.o: main.c aiv_renderer.h
	$(CC) -c -o $@ $(CFLAGS) $<

aiv_math.o: aiv_math.c aiv_math.h
	$(CC) -c -o $@ $(CFLAGS) $<

aiv_rasterizer.o: aiv_rasterizer.c aiv_rasterizer.h aiv_math.h
	$(CC) -c -o $@ $(CFLAGS) $<

