#include "ilomatrix.h"

IloMatrix::IloMatrix(unsigned int rows, unsigned int cols)
{
	mat.clear();
	mat.resize(rows);
	
	for(unsigned int i=0;i<mat.size();++i)
	{
		mat[i].resize(cols);
	}
}

void IloMatrix::addEqualityConstraint(IloEnv & env, IloModel & model,
	IloRangeArray & con, IloNumVarArray & x, IloNumArray b)
{
	for(unsigned int i=0;i<mat.size();++i)
	{
		IloExpr expr(env);
		for(unsigned int j=0;j<mat[i].size();++j)
		{
			expr += mat[i][j]*x[j];
		}
		con.add(expr == b[i] );
	}

	// add the constraints
	model.add( con );
}

IloNum& IloMatrix::operator() (unsigned int i, unsigned int j)
{
	return mat[i][j];
}

IloNumArray IloMatrix::scalProd(IloEnv & env, IloNumArray & x)
{
	IloNumArray prod(env, mat.size());
	for(unsigned int i=0;i<mat.size();++i)
	{
		for(unsigned int j=0;j<mat[i].size();++j)
		{
			prod[i] = mat[i][j]*x[i];
		}
	}

	return prod;
}