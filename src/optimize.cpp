#include "optimize.h"

IloExpr D(unsigned int i, Sparse_Matrix<IloInt>& vox, IloNumVarArray& w, IloEnv & env)
{
	IloExpr exp(env);

	for(unsigned int j=vox.ia[i];j<vox.ia[i+1];++j)
	{
		exp += vox.a[j]*w[vox.ja[j]];
	}
	/* old, not as efficient i think
	for(unsigned int j=0;j<vox.n;++j)
	{
		if(vox(i,j) != 0){
			exp += vox(i,j)*w[j];
		}
	}*/
	return exp;
}

IloNum D(unsigned int i, Sparse_Matrix<IloInt>& vox, IloNumArray& w_hat)
{
	IloNum num = 0;

	for(unsigned int j=vox.ia[i];j<vox.ia[i+1];++j)
	{
		num += vox.a[j]*w_hat[vox.ja[j]];
	}
	/* old not as efficient, but its purpose is more apparent
	for(unsigned int j=0;j<vox.n;++j)
	{
		if(vox(i,j) != 0){
			num += vox(i,j)*w_hat[j];
		}
	}*/
	return num;
}

void DRef( unsigned i, Sparse_Matrix<IloInt>& vox, IloNumVarArray& w, IloExpr & expr)
{
   for(unsigned int j=vox.ia[i];j<vox.ia[i+1];++j)
	{
		expr += vox.a[j]*w[vox.ja[j]];
	}
}


IloExpr twelveLHS(IloEnv & env, Sparse_Matrix<IloInt>& vox, vector<unsigned int> & A,
	IloNum Tx, vector<unsigned int> & X, IloNumVarArray& w)
{
	IloExpr expr(env);

	for(unsigned int i=0;i<A.size();++i)
	{
		expr += D(A[i], vox, w, env) - Tx;
	}

	return expr;
}

void twelveLHSRef(IloEnv & env, IloExpr & expr, Sparse_Matrix<IloInt>& vox, vector<unsigned int> & A,
	IloNum Tx, vector<unsigned int> & X, IloNumVarArray & w)
{
   for(unsigned int i=0;i<A.size();++i)
	{
      IloExpr temp_expr(env);
      DRef(A[i], vox, w, temp_expr);
		expr += temp_expr - Tx;
      temp_expr.end();
	}
}

IloExpr thirteenLHS(IloEnv & env, Sparse_Matrix<IloInt>& vox, vector<unsigned int> & B,
	IloNum Ty, vector<unsigned int> & Y, IloNumVarArray & w)
{
	IloExpr expr(env);

	for(unsigned int i=0;i<B.size();++i)
	{
		expr += Ty - D(B[i], vox, w, env);
	}

	return expr;
}

void thirteenLHSRef(IloEnv & env, IloExpr & expr, Sparse_Matrix<IloInt>& vox, vector<unsigned int> & B,
	IloNum Ty, vector<unsigned int> & Y, IloNumVarArray & w)
{
   for(unsigned int i=0;i<B.size();++i)
	{
      IloExpr temp_expr(env);
      DRef(B[i], vox, w, temp_expr);
		expr += Ty - temp_expr;
      temp_expr.end();
	}
}


Set getA(vector<IloNum> & Dx, Set & X, IloNum T)
{
	Set Ak;
	for(unsigned int i=0;i<Dx.size();++i)
	{
		if(Dx[i] >= T){
			Ak.push_back(X[i]);
		}
	}

	return Ak;
}

Set getB(vector<IloNum> & Dy, Set & Y, IloNum T)
{
	Set Bk;

	for(unsigned int i=0;i<Dy.size();++i)
	{
		if(Dy[i] <= T){
			Bk.push_back(Y[i]);
		}
	}

	return Bk;
}

vector<IloNum> getDoses(Sparse_Matrix<IloInt>& vox, IloNumArray & w, vector<unsigned> & organSet)
{
   vector<IloNum> doses(organSet.size());
   
   vector<IloNum>::iterator d_it = doses.begin();
   for(vector<unsigned>::iterator s_it=organSet.begin();
      s_it!=organSet.end();++s_it)
   {
      *d_it = D(*s_it, vox, w);
      ++d_it;
   }

   return doses;
}

IloExpr minDoses(IloEnv & env, IloNumVarArray & w, Sparse_Matrix<IloInt>& vox)
{
	IloExpr obj(env);

	for(int i=0;i<vox.a.size();++i)
	{
		obj += vox.a[i]*w[vox.ja[i]];
	}

	obj /= IloNum(vox.m);

	return obj;
}

IloExpr minBeamletStr(IloEnv & env, IloNumVarArray & w, Sparse_Matrix<IloInt> & vox)
{
	IloExpr obj(env);

	for(int i=0;i<w.getSize();++i)
	{
		obj += w[i]/IloNum(100);
	}

	return obj;
}
			

			
