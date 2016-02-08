#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <ncurses.h>

#include "ranlxd.h"
#include "lattice.h"
#include "metropolis.h"
#include "cluster.h"
#include "visual.h"
#include "input.h"

#define FNL 200


int main(int argc, char** argv) {

  input_t setup;
  lattice_t lat;

  parse_args(argc,argv,&setup);
  
  if(setup.seed == 0){
    printf("!! No RNG seed provided, seeding from time!\n");
    time_t t;
    time(&t);
    setup.seed = (unsigned)t;
  }
  
  srand(setup.seed);
  rlxd_init(2,setup.seed);

  init(&lat,&setup);

  // TODO: smart filenme
  FILE* ofile = fopen("output.data","w");
  if( (void*)ofile == NULL ){
    printf("There was an error opening the output file for writing!\n");
    exit(1);
  }

  if(setup.visual==1) initscr();

  double eperspin, magnetisation;
  int sweepsize;
  for(int n = 0; n < setup.nsweeps; ++n){
    sweepsize = lat.sweep(&lat);
    eperspin = H(&lat)/(2*lat.Lsq);
    magnetisation = M(&lat);
   
    fprintf(ofile,"%012d %.10e %.10e %012d\n",n,eperspin,fabs(magnetisation),sweepsize);

    if(setup.visual==1){
      clear();
      drawconf(&lat);
      plotenergy(eperspin,&lat);
      move(0,0);
      printw("Temperature: %.4e  Sweep: %d  Energy per spin: %.4e  Magnetisation: %.4e",lat.temp,n,eperspin,fabs(magnetisation));
      refresh();
      usleep(20000);
    }
    if(setup.algo == 's' && n == setup.ntherm && setup.visual == 1) getch();
    //if(setup->algo && n > ntherm){
      //T=Ts/log((float)(n-ntherm)*3);
    //  T=0.1+Ts*exp(-(float)(n-ntherm)/20.0);
    //}
  }
  
  if(setup.visual==1) {
    getch();
    endwin();
  }

  fclose(ofile);
  finalize(&lat);
}
