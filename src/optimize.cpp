#include "optimize.h"


/*  Opt class stuff */
Opt::Opt(IloEnv & env) : ArgMin(env), Duals(env)
{

}


void Opt::write(string fname)
{
	std::ofstream output(fname.c_str());

	if(!output.is_open()){
		cout << "Error opening " << fname << endl;
	}

	for(unsigned int i=0;i<ArgMin.getSize()-1;++i)
	{
		output << ArgMin[i] << "\n";
	}

	output << ArgMin[ArgMin.getSize()-1];

	output.close();
}


/* End Opt class stuff */

bool cuttingPlaneMethod(IloEnv & env, Opt& solution, Sparse_Matrix<IloInt> & vox,
	Set & X, Set & Y, const IloNum& Ux, const IloNum& Ly, Distribution& u, Distribution& v)
{
	// booleans for solutions
	bool solved = false;
	bool infeasible = false;
	bool reachedmaxits = false;

	IloInt n, m;
	m = vox.m;
	n = vox.n;

	// decision variable
	try{
		IloNumVarArray w(env, n, 0, 120);

		/******* Step 0 *********/
		unsigned int k, kx, ky;
		k = kx = ky = 1;
			
		while(!solved && !reachedmaxits && !infeasible)
		{
			/******* Step 1 *********/
			IloModel model(env);
			

			// objective function
			IloExpr obj = minDoses(env, w, vox);
			
			// add the objective function
			model.add( IloMinimize( env, obj) );

			// begin to add constraints
			IloRangeArray constraints(env);

			// (12)
			constraints.add( twelveLHS(env, vox, X, Ux, X, w) <= IloNum(X.size())*(u.intToInf(Ux)) );
			// (13)
			constraints.add( thirteenLHS(env, vox, Y, Ly, Y, w) <= IloNum(Y.size())*(v.intTo(Ly)) );

			model.add( constraints );

			cout << "solving" << endl;
			IloCplex cplex(model);
			cplex.setOut(env.getNullStream());		// silent
			cplex.setParam(IloCplex::WorkMem, 7000); // add memory
			//cplex.setParam(IloCplex::MemoryEmphasis, 1); // save memory
			//cplex.setParam(IloCplex::Threads, 1);  // set thread count

			while(!solved && !infeasible)
			{
				cout << "ITERATION " << k << endl;
		
				if( !cplex.solve() ){
					env.error() << "Failed to optimize LP." << endl;
					throw(-1);
				}

				// Get solution
				IloNum min = cplex.getObjValue();
				IloNumArray vals(env);
				IloNumArray prev(env);
				cplex.getValues(vals, w);

				// calculates the doses at the current solution
				vector<IloNum> Dx, Dy;
				Dx.resize(X.size()); Dy.resize(Y.size());
				for(unsigned int i=0;i<Dx.size();++i){
					Dx[i] = D(X[i], vox, vals);
				}

				for(unsigned int i=0;i<Dy.size();++i){
					Dy[i] = D(Y[i], vox, vals);
				}

				/****** Step 2 ******/
				IloNum dx, dy, Txk, Tyk;
				dx = deltaX(Txk, 0., X.size(), Dx, u);
				dy = deltaY(Tyk, 0., Y.size(), Dy, v);

				cout << " dx dy " << dx << " " << dy << endl;

				// check to see if solution is optimal
				if(dx <= EPSILON && dy <= EPSILON){
							
					cout << "\n\n\n" << endl;
					cout << "****************************************" << endl;
					cout << "A solution was found!" << endl;
					cout << k << " iterations" << endl;
					cout << kx << " cuts for the OAR constraint" << endl;
					cout << ky << " cuts for the PTV constraint" << endl;

					solution.Min = cplex.getObjValue();
					cplex.getValues( solution.ArgMin, w );

					return true;
				}
		
				/****** Step 3 ******/
				Set Ak, Bk;
				if(dx > 0.)
				{
					++kx;
					Ak = getA(Dx, X, Txk);
					model.add( twelveLHS(env, vox, Ak, Txk, X, w) <= IloNum(X.size())*u.intToInf(Txk) );
				}

				if(dy > 0.)
				{
					++ky;
					Bk = getB(Dy, Y, Tyk);
					model.add( thirteenLHS(env, vox, Bk, Tyk, Y, w) <= IloNum(Y.size())*v.intTo(Tyk) );
				}

				if(k == MAX_ITS){
					cout << "Reached max iterations " << endl;
					reachedmaxits = true;
					break;
				}

				++k;
			} // adding cuts
		}
	} // try
	catch(IloException& e){
		cout << "Concert exception caught: " << e << endl;
		infeasible = true;
		return false;
	}
	catch(...){
		cout << "Unknown exception caught" << endl;
		infeasible = true;
		return false;
	}
	return false;
}

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
			

			
