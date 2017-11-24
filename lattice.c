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
#include <stdio.h>

#include "lattice.h"

#include "ranlxs.h"
#include "metropolis.h"
#include "cluster.h"
#include "input.h"

#include "lattice_inlinedefs.ih"


double H(const lattice_t * const lat){
  int h = 0;
  int xm1, xp1, ym1, yp1;
  for(int x = 0; x < lat->L; ++x){
    shiftm(x,&xm1,lat);
    shiftp(x,&xp1,lat);
    for(int y = 0; y < lat->L; ++y){
      shiftm(y,&ym1,lat);
      shiftp(y,&yp1,lat);
      h += lat->spins[x][y] * ( lat->spins[xm1][y] + lat->spins[xp1][y] + lat->spins[x][ym1] + lat->spins[x][yp1] );
    }
  }
  // undo double counting
  return( -0.5*lat->J*h );
}

double M(const lattice_t * const lat){
  double m = 0.0;
  for(int i = 0; i < lat->Lsq; ++i){
    m += (int)lat->spinsmem[i];
  }
  return(m/lat->Lsq);
}

double singleflip_deltaH(const int xflip, const int yflip, const lattice_t * const lat){
  int xm1, xp1, ym1, yp1;
  shiftm( xflip, &xm1, lat );
  shiftm( yflip, &ym1, lat );
  shiftp( xflip, &xp1, lat );
  shiftp( yflip, &yp1, lat );
  // change of energy is -2 * (initial energy of spin pairs which include spin to be flipped)
  // (note that H has a minus sign!)
  return( 2*lat->J*lat->spins[xflip][yflip] * ( lat->spins[xm1][yflip] + lat->spins[xp1][yflip] + 
                                                lat->spins[xflip][ym1] + lat->spins[xflip][yp1] ) );
}

void init(lattice_t * const lat, input_t * const setup){
  lat->setup = *(setup);
  lat->L = setup->L;
  lat->Lsq = setup->L*setup->L;
  lat->J = setup->J;
  lat->temp = setup->temp;
  switch(setup->algo){
    case 'n':
      lat->sweep = null_sweep;
      break;
    case 'r':
      lat->sweep = random_sweep;
      break;
    case 'm':
      lat->sweep = metropolis_sweep;
      break;
    case 'c':
      lat->sweep = cluster_sweep;
      break;
    case 's':
    default:
      printf("lattice init: unsupported algorithm %c\n",setup->algo);
      exit(ERROR_INIT_ALGO);
      break;
  }

  lat->spinsmem = NULL;
  lat->spins = NULL;
  lat->clustermem = NULL;
  lat->cluster = NULL;

  // allocate memory for lattice and cluster                                                                                                                                       
  lat->spinsmem = malloc(lat->L*lat->L*sizeof(char));
  lat->spins = malloc(lat->L*sizeof(char*));
  if( (void*)lat->spinsmem == NULL || (void*)lat->spins == NULL ){
    printf("Memory allocation for spins failed!\n");
    exit(ERROR_INIT_SPINMEM);
  }
  for(int i = 0; i < lat->L; ++i)
    lat->spins[i] = &lat->spinsmem[i*lat->L];

  // perhaps the cluster memory should be part of the cluster algorithm...
  if(lat->sweep == cluster_sweep){
    lat->clustermem = malloc(lat->L*lat->L*sizeof(char));
    lat->cluster = malloc(lat->L*sizeof(char*));
    if( (void*)lat->clustermem == NULL || (void*)lat->cluster == NULL ){
      printf("Memory allocation for cluster failed!\n");
      exit(ERROR_INIT_CLUSTERMEM);
    }
    for(int i = 0; i < lat->L; ++i)
      lat->cluster[i] = &lat->clustermem[i*lat->L];
  } else {
    lat->clustermem = NULL;
    lat->cluster = NULL;
  }

  start(lat, setup->coldstart);

  lat->init = 1;
}

void start(lattice_t * const lat, const int coldstart){
  if(coldstart){
    for(int i = 0; i < lat->Lsq; ++i){
      lat->spinsmem[i] = -1;
    }
    // test code for symmetries
    //for(int x = 0; x < lat->L; ++x){
    //  for(int y = 0; y < lat->L; y++){
    //    if( x == y ){
    //      lat->spins[x][y] = -1;
    //    }else{
    //      lat->spins[x][y] = 1;
    //    }
    //  }
    //}
  } else {
    float* rans = malloc(lat->Lsq*sizeof(float));
    ranlxs(rans,lat->Lsq);
    for(int i = 0; i < lat->Lsq; ++i)
      lat->spinsmem[i] = rans[i] > 0.5 ? 1 : -1;
    free(rans);
  }
}

int null_sweep(lattice_t * const lat){
  return 0;
}

int random_sweep(lattice_t * const lat){
  start(lat, 0);
  return lat->Lsq;
}

void finalize(lattice_t * const lat) {
  if(lat->init == 1){
    if((void*)lat->cluster != NULL) free(lat->cluster);
    if((void*)lat->clustermem != NULL) free(lat->clustermem);
    if((void*)lat->spins != NULL) free(lat->spins);
    if((void*)lat->spinsmem != NULL) free(lat->spinsmem);
    lat->init = 0;
  }
}
