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

