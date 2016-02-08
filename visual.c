#include <ncurses.h>

#include "visual.h"
#include "lattice.h"

void drawconf(const lattice_t * const lat){
  for(int i = 0; i < lat->L; ++i){
    for(int j = 0; j < lat->L; ++j){
      move(i+VOFFSET,j);
      addch(lat->spins[i][j] > 0 ? '+' : ' ');
    }
  }
}

void plotenergy(const double e, const lattice_t * const lat){
  // keep history of last HISTWIDTH energies
  static double ehist[HISTWIDTH];
  static int init = 0;
  float maxe = 1.2;
  if(init == 0){
    for(int i = 0; i < HISTWIDTH; ++i)
      ehist[i] = 0;
    init = 1;
  }
  for(int i = 0; i < HISTWIDTH-1; ++i){
    ehist[i] = ehist[i+1];
  }
  ehist[HISTWIDTH-1] = e;
  
  for(int i = 0; i < HISTWIDTH; ++i){
    move(lat->L/2+VOFFSET-(int)(ehist[i]*lat->L/4/maxe),lat->L+i+1);
    addch('*');
  }
  move(0,lat->L+HISTWIDTH+5);
  printw("eperspin");
  for(int i = 0; i < lat->L+1; i+=2){
    move(i+VOFFSET,lat->L+HISTWIDTH+5);
    printw("%.4e",2*maxe*(1.0-2*(float)i/lat->L));
  }
}

