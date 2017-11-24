/*
 *   Copyright (C) 2017 Bartosz Kostrzewa
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


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <ncurses.h>

#include "ranlxs.h"
#include "lattice.h"
#include "metropolis.h"
#include "cluster.h"
#include "visual.h"
#include "input.h"

int main(int argc, char** argv) {

  input_t setup;
  lattice_t lat;
  lattice_t lat_temp;

  parse_args(argc,argv,&setup,INPUT_MODE_SYM);
  
  if(setup.seed == 0){
    printf("!! No RNG seed provided, seeding from time!\n");
    time_t t;
    time(&t);
    setup.seed = (unsigned)t;
  }
  
  srand(setup.seed);
  rlxs_init(2,setup.seed);

  init(&lat,&setup);
  init(&lat_temp,&setup);

  lattice_copy(&lat_temp, &lat);

  FILE* ofile = fopen(setup.ofilename,"w");
  if( (void*)ofile == NULL ){
    printf("There was an error opening the output file '%s' for writing!\n",setup.ofilename);
    exit(1);
  }

  if(setup.visual==1) initscr();

  double eperspin, magnetisation;
  int sweepsize;
  for(int n = 0; n < setup.nsweeps; ++n){
    sweepsize = lat.Lsq;
    eperspin = H(&lat)/(lat.Lsq);
    magnetisation = M(&lat);
   
    fprintf(ofile,"%012d %.10e %.10e %012d\n",n,eperspin,fabs(magnetisation),sweepsize);

    clear();
    drawconf(&lat);
    if(setup.plotenergy==1) plotenergy(eperspin,&lat);
    move(0,0);
    printw("Temperature: %.4e  Sweep: %d  Energy per spin: %.4e",lat.temp,n,eperspin);
    move(1,0);
    printw("Magnetisation: %.4e",fabs(magnetisation));

    move(1,27);
    if( n < 20 ){
      lattice_translation(&lat, &lat_temp, -1, -1);
      printw("Symmetry: Translation by (1,1)       (press any key to advance)");
    } else if(n < 40){ 
      lattice_90deg_rotation(&lat, &lat_temp);
      printw("Symmetry: Rotation by 90deg          (press any key to advance)");
    } else if(n < 60){
      lattice_reflection(&lat, &lat_temp);
      printw("Symmetry: reflection along diagonal  (press any key to advance)");
    } else if(n < 80){
      lattice_spinflip(&lat, &lat_temp);
      printw("Symmetry: global spin flip           (press any key to advance)");
    }
    lattice_copy(&lat_temp, &lat);

    refresh();
    getch();
  }
  
  if(setup.visual==1) {
    move(1,27);
    printw("PRESS ANY KEY TO EXIT PROGRAM                                       ");
    getch();
    endwin();
  }

  fclose(ofile);
  finalize(&lat);
}
