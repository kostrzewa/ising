#ifndef INPUT_H
#define INPUT_H

typedef struct input_t {
  int L;
  double J;
  double temp;
  char algo;
  int ntherm;
  int nsweeps;
  int visual;
  int seed;
  int coldstart;
} input_t; 

#include "lattice.h"

void parse_args(int argc, char** argv, input_t * const setup);
void usage();

#endif
