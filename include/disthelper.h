#pragma once

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include "sparse_matrix.h"
#include "set.h"
#include "optimize.h"


// helper class
class DistHelper
{
public:
	DistHelper(Sparse_Matrix<IloInt> *v, Set * x, Set * y, IloNumArray *what);
	// calculates card{ i \in X : D(i) <= t } / card(X) , F(t)
	IloNum distOnX( IloNum t);
	// G(t)
	IloNum distOnY(IloNum t);
	// Calculates the integral of these dists
	IloNum intDistOnX(IloNum T);
	IloNum intDistOnY(IloNum T);
	void print(string fname, IloNum lower, IloNum upper, IloNum count);
private:
	Sparse_Matrix<IloInt> *vox;
	Set *X;
	Set *Y;
	IloNumArray * w;
};

