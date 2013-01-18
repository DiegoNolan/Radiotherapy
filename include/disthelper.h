#pragma once

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include "sparse_matrix.h"
#include "set.h"
#include "optimize.h"


// helper class for returning distrubitions of the regions
class DistHelper
{
public:
   DistHelper(){ vox = NULL; Region = NULL; w = NULL;}
	DistHelper(Sparse_Matrix<IloInt> *v, Set * region, IloNumArray *what);
   void operator= (DistHelper & arg);
   // distribution of the region
	IloNum dist( IloNum t);
	// Calculates the integral of these dists
	IloNum intToInf(IloNum T);
	IloNum intTo(IloNum T);
//	void print(string fname, IloNum lower, IloNum upper, IloNum count);
private:
	Sparse_Matrix<IloInt> *vox;
	Set *Region;
	IloNumArray * w;
};

