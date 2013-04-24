#ifndef VISUALBENCHMARK_H
#define VISUALBENCHMARK_H

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <set>
#include "set.h"
#include "optimize.h"

using std::set;

// Generate a random set for ptv and oar between the range and inside grid of given size
void randomSetAndPTV(Set & ptv, Set & oar, unsigned rows, unsigned cols,
   unsigned lowerPTV, unsigned upperPTV, unsigned lowerOAR, unsigned upperOAR);



#endif // VISUALBENCHMARK_H
