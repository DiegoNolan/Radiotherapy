#ifndef ILOMATRIX_H
#define ILOMATRIX_H

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN
#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

class IloMatrix
{
public:
	IloMatrix(unsigned int rows=0, unsigned int cols=0);
	~IloMatrix(){};
	// such that Ax = b, where A is mxn and x and b are vectors
	// sets con equal to an array of the constraints
	void addEqualityConstraint(IloEnv & env, IloModel & model,
		IloRangeArray & con, IloNumVarArray & x, IloNumArray b);
	unsigned int getRows(){return mat.size();}
	unsigned int getCols(){return mat[0].size();}
	IloNum& operator() (unsigned int i, unsigned int j);
	// returns the scalar prod of Ax, not xA
	IloNumArray scalProd(IloEnv & env, IloNumArray & x);
private:
	vector< vector<IloNum> > mat;
};


#endif // ILOMATRIX_H