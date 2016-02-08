#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "input.h"
#include "errors.h"

void parse_args(int argc, char** argv, input_t * const setup){
  setup->seed = 0;
  setup->L = 0;
  setup->temp = 0;
  setup->J = 0;
  setup->algo = 'n';
  setup->nsweeps = 0;
  setup->ntherm = 0;
  setup->visual = 0;
  setup->coldstart = 0;

  int c;
  // if you have never worked with getopt, the argument value is written into
  // the global string 'optarg' managed by getopt
  // an argument followed with a colon has a value, otherwise it's just a flag
  while((c = getopt(argc, argv, "h?vcJ:L:T:a:t:n:s:")) != -1) {
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
        if( optarg[0] == 'm' || optarg[0] == 'c' || optarg[0] == 's' ){
          setup->algo = optarg[0];
        } else {
          printf("%s is not a valid algorithm!\n",optarg);
          usage();
          exit(ERROR_INPUT_ALGO);
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
      case 'c':
        setup->coldstart = 1;
        break;
      case 'h':
      case '?':
      default:
        usage();
        break;
    }
  }
  // do some sanity checks
  if( setup->L <= 0 ){
    printf("%d is not a valid lattice size!\n",setup->L);
    usage();
    exit(ERROR_INPUT_ARGC);
  }
  if( setup->nsweeps <= 0 ){
    printf("%d is not a valid number of sweeps!\n",setup->nsweeps);
    usage();
    exit(ERROR_INPUT_ARGC);
  }
  if( setup->ntherm < 0 || setup->ntherm >= setup->nsweeps ){
    printf("%d is not a valid number of thermalisation sweeps!\n",setup->ntherm);
    usage();
    exit(ERROR_INPUT_ARGC);
  }
  if( setup->seed < 0 ){
    printf("%d is not a valid RNG seed!\n",setup->seed);
    usage();
    exit(ERROR_INPUT_ARGC);
  }
}

void usage() {
  printf("'ising' command line arguments\n");
  printf("./ising -L<L> -J<J> -T<T> -a<a> -n<n> [-t<t>] [-s<s>] [-c] [-v]\n");
  printf("MANDATORY ARGUMENTS\n");
  printf("<L> lattize size [integer]\n");
  printf("<J> spin coupling [decimal]\n");
  printf("<T> (starting) temperature [decimal]\n");
  printf("<a> algorithm: (c)luster (m)etropolis (s)imulated annealing\n"); 
  printf("<n> total number of sweeps [integer]\n");
  printf("OPTIONAL ARGUMENTS\n");
  printf("<t> thermalistion sweeps for simulated annealing [integer < n ]\n");
  printf("<s> RNG seed [integer]\n");
  printf("-c cold start (all spins aligned)\n");
  printf("-v enable visualisation (and slow down simulation)\n");
}
 
