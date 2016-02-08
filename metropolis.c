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
