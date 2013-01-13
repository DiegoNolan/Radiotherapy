#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN
#include "sparse_matrix.h"
#include "distribution.h"
#include "set.h"
#include "opt.h"

#define NUM_T_VALS 1000
#define NUM_T_FOR_DC 100
#define MAX_ITS 10000
#define EPSILON 0.000001
#define MAX(A,B) ( (A) > (B) ? (A) : (B))

// From The Cutting plane algorithm from Stochastic Programming Approaches for Radiation Therapy Planning
bool cuttingPlaneMethod(IloEnv & env, Opt& solution, Sparse_Matrix<IloInt> & vox,
	Set & X, Set & Y, const IloNum& Ux, const IloNum& Ly, Distribution& u, Distribution& v);

// returns the expression for the dose
IloExpr D(unsigned int i, Sparse_Matrix<IloInt>& vox, IloNumVarArray& w, IloEnv & env);
IloNum D(unsigned int i, Sparse_Matrix<IloInt>& vox, IloNumArray& w_hat);

// returns \sum _{i \in A} (D(i)-Tx) 
IloExpr twelveLHS(IloEnv & env, Sparse_Matrix<IloInt>& vox, vector<unsigned int> & A,
	IloNum Tx, vector<unsigned int> & X, IloNumVarArray & w);

// returns \sum _{i \in B}(Ty-D(i)
IloExpr thirteenLHS(IloEnv & env, Sparse_Matrix<IloInt>& vox, vector<unsigned int> & B,
	IloNum Ty, vector<unsigned int> & Y, IloNumVarArray & w);

// returns the max _{t >= U} ( \max (0, D(i) - t) - card(X)*u(t-z) )
template <class Dist>
IloNum deltaX(IloNum & t, IloNum z, unsigned int cardX,
	vector<IloNum> & Dxk, Dist & u)
{
	vector<IloNum> t_range = u.getTRange(NUM_T_VALS);

	IloNum max = -FLT_MAX;
	
	for(unsigned int it=0;it<t_range.size();++it)
	{
		IloNum val=0;
		for(unsigned int i=0;i<Dxk.size();++i)
		{
			val += MAX( 0.f, Dxk[i] - t_range[it]);
		}

		val -= IloNum(cardX)*u.intToInf(t_range[it]-z);

		if(val >= max){
			max = val;
			t = t_range[it];
		}
	}

	return max;
}
// returns the max _{0 <= t <= Ly} ( \max (0, t - D(i)) -card(Y)v(t+s) )
template <class Dist>
IloNum deltaY(IloNum & t, IloNum s, unsigned int cardY,
	vector<IloNum> & Dyk, Dist & v)
{
	vector<IloNum> t_range = v.getTRange(NUM_T_VALS);

	IloNum max = -FLT_MAX;
	
	for(unsigned int it=0;it<t_range.size();++it)
	{
		IloNum val=0;
		for(unsigned int i=0;i<Dyk.size();++i)
		{
			val += MAX( 0.f, t_range[it] - Dyk[i]);
		}

		val -= IloNum(cardY)*v.intTo(t_range[it]+s);

		if(val >= max){
			max = val;
			t = t_range[it];
		}
	}

	return max;
}

// A = {i \in X: D^k(i) >= T^{k_x)_X }
Set getA(vector<IloNum> & Dx, Set & X, IloNum T);
// A = {i \in Y: D^k(i) <= T^{k_y}_Y }
Set getB(vector<IloNum> & Dy, Set & Y, IloNum T);

/******** Objective Functions *****************/
IloExpr minDoses(IloEnv & env, IloNumVarArray & w, Sparse_Matrix<IloInt>& vox);
IloExpr minBeamletStr(IloEnv & env, IloNumVarArray & w, Sparse_Matrix<IloInt> & vox);



#endif // OPTIMIZE_H