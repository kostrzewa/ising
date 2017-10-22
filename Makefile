PGMS := ising_mc ising_sym 
MODULES := ranlxd lattice metropolis cluster visual input
HEADERS := $(addsuffix .h, $(MODULES)) errors.h
LDFLAGS := -L/usr/lib/ -lm -lncurses
IDIRS := /usr/include/ncursesw .

CC=gcc
CFLAGS=-std=c99 -O3 -mtune=core2 -march=core2 -fopenmp -pg -g -ggdb

all: ${PGMS}

%.o: %.c $(HEADERS) Makefile
	$(CC) -c -o $@ $< $(addprefix -I, $(IDIRS)) $(CFLAGS)

$(PGMS): % : %.o $(addsuffix .o,$(MODULES))
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)



clean:
	rm -f *.o ${PGMS}

.PHONY: clean all

