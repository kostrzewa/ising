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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#include "input.h"
#include "errors.h"

void parse_args(int argc, char** argv, input_t * const setup, const input_mode_t imode){
  setup->seed = 0;
  setup->L = 0;
  setup->temp = 0;
  setup->J = 0;
  setup->algo = 'n';
  setup->nsweeps = 0;
  setup->ntherm = 0;
  setup->visual = 0;
  setup->plotenergy = 0;
  setup->coldstart = 0;
  setup->iterate = 0;
  strncpy(setup->ofilename, "output.data", FNL);

  int c;
  // if you have never worked with getopt, the argument value is written into
  // the global string 'optarg' managed by getopt
  // an argument followed with a colon has a value, otherwise it's just a flag
  while((c = getopt(argc, argv, "h?vceiJ:L:T:a:t:n:s:o:")) != -1) {
    switch(c) {
      case 'L':
        setup->L = atoi(optarg);
        break;
      case 'T':
        setup->temp = atof(optarg);
        break;
      case 'J':
        setup->J = atof(optarg);
        break;
      case 'a':
        if(imode == INPUT_MODE_MC){
          if( optarg[0] == 'm' || optarg[0] == 'c' || optarg[0] == 's' || optarg[0] == 'r' ){
            setup->algo = optarg[0];
          } else {
            printf("%s is not a valid algorithm!\n",optarg);
            usage(imode);
            exit(ERROR_INPUT_ALGO);
          }
        }
        break;
      case 'n':
        setup->nsweeps = atoi(optarg);
        break;
      case 't':
        setup->ntherm = atoi(optarg);
        break;
      case 'v':
        setup->visual = 1;
        break;
      case 'e':
        setup->plotenergy = 1;
        break;
      case 'i':
        setup->iterate = 1;
        break;
      case 'c':
        setup->coldstart = 1;
        break;
      case 'o':
        strncpy(setup->ofilename, optarg, FNL);
        break;
      case 'h':
      case '?':
      default:
        usage(imode);
        break;
    }
  }

  if(imode == INPUT_MODE_SYM){
    setup->visual = 1;
    setup->iterate = 1;
    setup->nsweeps = 80;
    setup->coldstart = 0;
    setup->plotenergy = 1;
  }

  // do some sanity checks
  if( setup->L <= 0 ){
    printf("%d is not a valid lattice size!\n",setup->L);
    usage(imode);
    exit(ERROR_INPUT_ARGC);
  }
  if( setup->temp < 0 ){
    printf("%lf is not a valid temperature!\n",setup->temp);
    usage(imode);
    exit(ERROR_INPUT_ARGC);
  }
  if( setup->seed < 0 ){
    printf("%d is not a valid RNG seed!\n",setup->seed);
    usage(imode);
    exit(ERROR_INPUT_ARGC);
  }

  if(imode == INPUT_MODE_MC){
    if( setup->algo == 'n' ){
      printf("You must specify an algorithm for Monte Carlo!\n");
      usage(imode);
      exit(ERROR_INPUT_ARGC);
    }
    if( setup->nsweeps <= 0 ){
      printf("%d is not a valid number of sweeps!\n",setup->nsweeps);
      usage(imode);
      exit(ERROR_INPUT_ARGC);
    }
    if( setup->ntherm < 0 || setup->ntherm >= setup->nsweeps ){
      printf("%d is not a valid number of thermalisation sweeps!\n",setup->ntherm);
      usage(imode);
      exit(ERROR_INPUT_ARGC);
    }
  }
}

void usage(const input_mode_t imode) {
  if(imode == INPUT_MODE_MC){
    printf("'ising_mc' command line arguments\n");
    printf("./ising_mc -L<L> -J<J> -T<T> -a<a> -n<n> [-o<o>] [-t<t>] [-s<s>] [-c] [-v] [-e] [-i]\n");
  } else if( imode == INPUT_MODE_SYM ){
    printf("'ising_sym' command line arguments\n");
    printf("./ising_sym -L<L> -J<J> -T<T> [-o<o>]\n");
  }
  printf("MANDATORY ARGUMENTS\n");
  printf("<L> lattize size [integer]\n");
  printf("<J> spin coupling [decimal]\n");
  printf("<T> (starting) temperature [decimal]\n");
  if(imode == INPUT_MODE_MC){
    printf("<a> algorithm: (r)andom sampling, (c)luster, (m)etropolis, (s)imulated annealing\n"); 
    printf("<n> total number of sweeps [integer]\n");
    printf("OPTIONAL ARGUMENTS\n");
    printf("<o> output filename [character string, default 'output.data']\n");
    printf("<t> thermalistion sweeps for simulated annealing [integer < n ]\n");
    printf("-v enable visualisation (and slow down simulation)\n");
    printf("-e enable plotting of energy history in visualisation\n");
    printf("-i interactively iterate through simulation (when visualisation is enabled)\n");
  }else{
    printf("OPTIONAL ARGUMENTS\n");
    printf("<o> output filename [character string, default 'output.data']\n");
  }
  printf("<s> RNG seed [integer]\n");
  printf("-c cold start (all spins aligned)\n");
}
 
