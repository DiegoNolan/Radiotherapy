#ifndef VISUALBENCHMARK_H
#define VISUALBENCHMARK_H

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include <vector>
#include "set.h"

void randomSetAndPTV(Set & X, Set & Y, unsigned rows, unsigned cols,
   unsigned lowerPTV, unsigned upperPTV, unsigned lowerOAR, unsigned upperOAR);


#endif // VISUALBENCHMARK_H
