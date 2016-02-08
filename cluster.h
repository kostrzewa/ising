#ifndef CLUSTER_H
#define CLUSTER_H

#include "lattice.h"

typedef struct neighbour_t {
  int x;
  int y;
} neighbour_t;

typedef enum cluster_state_t {
  CLUSTER_UNCHECKED=0,
  CLUSTER_CHECKED,
  CLUSTER_RECURSE,
  CLUSTER_FLIP 
} cluster_state_t;

int cluster_sweep(lattice_t * const lat);
void check_neighbours(const int x, const int y, lattice_t * const lat);

#endif
