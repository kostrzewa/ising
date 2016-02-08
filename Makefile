PGMS=ising
MODULES=ranlxd lattice metropolis cluster visual input
HEADERS=$(addsuffix .h, $(MODULES)) errors.h
LDFLAGS=-L/usr/lib/ -lm -lncurses
IDIRS=/usr/include/ncursesw .

CC=gcc
CFLAGS=-std=c99 -O3 -mtune=core2 -march=core2 -fopenmp

%.o: %.c $(HEADERS) Makefile
	$(CC) -c -o $@ $< $(addprefix -I, $(IDIRS)) $(CFLAGS)

$(PGMS): $(addsuffix .o,$(PGMS)) $(addsuffix .o,$(MODULES))
	gcc -o $@ $^ $(LDFLAGS) $(CFLAGS)

all: ising

clean:
	rm -f *.o ising

.PHONY: clean all

