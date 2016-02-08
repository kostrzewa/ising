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


#ifndef LATTICE_H
#define LATTICE_H

#include "input.h"
#include "errors.h"

typedef struct lattice_t {
  int L;
  int Lsq;
  double temp;
  double J;

  int init;

  char * spinsmem;
  char ** spins;
  char *clustermem;
  char ** cluster;
  /* function pointer to function which takes a pointer to a lattice_t struct as an argument
   * because lattice_t is an unknown type at this point, need to keep 'struct' syntax */ 
  int (*sweep)(struct lattice_t * lat);
} lattice_t;

void init(lattice_t * const lat, input_t * const setup);
void finalize(lattice_t * const lat);

double H(const lattice_t * const lat);
double M(const lattice_t * const lat);

double singleflip_deltaH(const int xflip, const int yflip, const lattice_t * const lat);

inline void shiftm(const int in, int * const out){
  *out = in - 1;
  *out = *out < 1 ? 0 : *out;
}

inline void shiftp(const int in, int * const out, const lattice_t * const lat){
  *out = (in + 1) % lat->L;
}


#endif
