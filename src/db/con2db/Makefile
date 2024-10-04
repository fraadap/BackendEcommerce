CC=g++
CPPFLAGS=-std=c++11 -I. -I /usr/include/postgresql
LDFLAGS=-lpq
#CFLAGS=-lm

DEPS = $(wildcard *.h)
objects := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

.PHONY: clean

all: $(objects)

%.o: %.cpp $(DEPS) 
	$(CC) -c -o $@ $< $(CPPFLAGS) $(LDFLAGS)

clean:
	rm -f *.o
