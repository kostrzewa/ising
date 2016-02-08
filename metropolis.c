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


#include <stdlib.h>
#include <math.h>

#include "metropolis.h"
#include "ranlxd.h"
#include "lattice.h"

int metropolis_sweep(lattice_t * const lat){
  int xflip, yflip, acceptedflips;
  double e, deltae, test;
  double ranflip[2];
  acceptedflips = 0;
  for(int i = 0; i < lat->Lsq; ++i){
    ranlxd(ranflip,2);
    xflip = (int)(ranflip[0]*lat->L);
    yflip = (int)(ranflip[1]*lat->L);
    ranlxd(&test,1);
    deltae = singleflip_deltaH(xflip,yflip,lat);
    // accept or reject spin flip
    if( exp( -((double)(deltae))/lat->temp ) > test ){
      lat->spins[xflip][yflip] = -lat->spins[xflip][yflip];
      ++acceptedflips;
    }
  }
  // return lattice energy after sweep
  return acceptedflips;
}
