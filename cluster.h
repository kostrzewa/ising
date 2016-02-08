/*
 *   Copyright (C) 2016 Bartosz Kostrzewa
 *
 *   This file is part of ising, a simple simultion code for the
 *   2D Ising model.
 *
 *   ising is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   ising is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with ising.  If not, see <http://www.gnu.org/licenses/>. 
 *
 */


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
