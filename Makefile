
CC := g++
OUT := bin/out
EXT := cpp
SRCDIR := src
LIBDIR := lib
TSTDIR := test

SOURCES := $(shell find $(SRCDIR) -type f -name *.$(EXT))
DEPENDS := $(shell find $(LIBDIR) -type f -name *.$(EXT))
TESTS := $(shell find $(TSTDIR) -type f -name *.$(EXT))

all:
	$(CC) -o $(OUT) $(SOURCES) $(DEPENDS)
	./$(OUT)
tests:
	$(CC) -o $(OUT) $(TESTS) $(DEPENDS)
	./$(OUT)
	rm -f $(OUT)