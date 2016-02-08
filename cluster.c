#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include <stdio.h>

#include "cluster.h"
#include "ranlxd.h"
#include "lattice.h"

int cluster_sweep(lattice_t * const lat){
  int xseed, yseed, clustersize;
  double seedran[2];
 
  // reset the cluster
  for(int i = 0; i < lat->Lsq; ++i){
    lat->clustermem[i] = CLUSTER_UNCHECKED;
  }
  // pick a seed point
  ranlxd(seedran,2);
  xseed = (int)(seedran[0]*(lat->L));
  yseed = (int)(seedran[1]*(lat->L));

  lat->cluster[xseed][yseed] = CLUSTER_RECURSE;
  // enter recursive function which iterates of the neighbours
  check_neighbours(xseed,yseed,lat);

  // flip the cluster
  clustersize = 0;
  for(int i = 0; i < lat->Lsq; ++i){
    if( lat->clustermem[i] == CLUSTER_FLIP ) {
      lat->spinsmem[i] *= -1;
      ++clustersize;
    }
  }
  
  return clustersize;
}

void check_neighbours(const int x, const int y, lattice_t * const lat){

  // if we are in a recursion, swtich state of origin cluster member to FLIP so that it won't be changed again
  if(lat->cluster[x][y]==CLUSTER_RECURSE) lat->cluster[x][y] = CLUSTER_FLIP;

  neighbour_t n[4];
  int xm1, xp1, ym1, yp1;
  double test;
  shiftm(x,&xm1);
  shiftm(y,&ym1);
  shiftp(x,&xp1,lat);
  shiftp(y,&yp1,lat);
 
  n[0].x = xm1; n[0].y = y;
  n[1].x = xp1; n[1].y = y;
  n[2].x = x;   n[2].y = ym1;
  n[3].x = x;   n[3].y = yp1;

  // iterate over neighbours
  for(int i = 0; i < 4; ++i){
    // if a neighbour has not yet been visited, see if it is parallel
    if( lat->cluster[ n[i].x ][ n[i].y ] == CLUSTER_UNCHECKED && lat->spins[ n[i].x ][ n[i].y ] == lat->spins[x][y] ){
      // and add with Wolff probability
      ranlxd(&test,1);
      if( (1.0-exp(-2*lat->J/lat->temp)) > test ){
        lat->cluster[ n[i].x ][ n[i].y ] = CLUSTER_RECURSE;
      }
    } 
  }

  // recurse to each neighbour which has been added to the cluster
  for(int i = 0; i < 4; ++i){
    if( lat->cluster[ n[i].x ][ n[i].y ] == CLUSTER_RECURSE ){
      check_neighbours( n[i].x, n[i].y, lat );
    }
  }

  return;
}

