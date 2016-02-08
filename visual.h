#ifndef VISUAL_H
#define VISUAL_H

#include "lattice.h"

#define VOFFSET 2
#define HISTWIDTH 50

void drawconf(const lattice_t * const lat);
void plotenergy(const double e, const lattice_t * const lat);

#endif
