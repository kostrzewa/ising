#ifndef ERRORS_H
#define ERRORS_H

typedef enum errors_t {
  ERROR_NONE = 0,
  ERROR_INPUT_ARGC,
  ERROR_INPUT_ALGO,
  ERROR_INIT_SPINMEM,
  ERROR_INIT_CLUSTERMEM,
  ERROR_INIT_ALGO
} errors_t;

#endif
