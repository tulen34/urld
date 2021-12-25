CC ?= /bin/cc
CFLAGS = -g -Wall -Wextra -Wconversion -Wstrict-prototypes
LDLIBS = -lcurl

SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c, obj/%.o, $(SOURCES))
BINARY = bin/urldisc

.PHONY: all build clean run

all: clean build run

obj:
	mkdir -p obj

bin:
	mkdir -p bin

obj/%.o: %.c obj
	@echo 'Compiling object file to $@ from $<'
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLIBS)

build: $(OBJECTS) bin
	@echo 'Compiling binary file'
	$(CC) $(CFLAGS) $(LDLIBS) -o $(BINARY) $(OBJECTS)

clean:
	@echo 'Cleaning'
	rm -rf bin obj

run:
	@echo 'Running $(BINARY)'
	@./$(BINARY)
