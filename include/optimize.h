#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include <map>
#include <string>
#include <omp.h>

#include "sparse_matrix.h"
#include "distribution.h"
#include "set.h"
#include "opt.h"
#include "benchmark.h"
 
using std::map;
using std::string;

#define MAX_ITS 100000
#define EPSILON 10
#define MAX(A,B) ( (A) > (B) ? (A) : (B))

// used for exceeding of the benchmarks
typedef struct
{
   IloNum upper, lower, t_upper, t_lower;
} Delta;


// returns the expression for the dose
IloExpr D(unsigned int i, Sparse_Matrix<IloInt>& vox, IloNumVarArray& w, IloEnv & env);
IloNum D(unsigned int i, Sparse_Matrix<IloInt>& vox, IloNumArray& w_hat);
// do by reference
void DRef( unsigned i, Sparse_Matrix<IloInt>& vox, IloNumVarArray& w, IloExpr & expr);

// returns \sum _{i \in A} (D(i)-Tx) 
IloExpr twelveLHS(IloEnv & env, Sparse_Matrix<IloInt>& vox, vector<unsigned int> & A,
	IloNum Tx, vector<unsigned int> & X, IloNumVarArray & w);
// reference
void twelveLHSRef(IloEnv & env, IloExpr & expr, Sparse_Matrix<IloInt>& vox, vector<unsigned int> & A,
	IloNum Tx, vector<unsigned int> & X, IloNumVarArray & w);

// returns \sum _{i \in B}(Ty-D(i)
IloExpr thirteenLHS(IloEnv & env, Sparse_Matrix<IloInt>& vox, vector<unsigned int> & B,
	IloNum Ty, vector<unsigned int> & Y, IloNumVarArray & w);
// refernce
void thirteenLHSRef(IloEnv & env, IloExpr & expr, Sparse_Matrix<IloInt>& vox, vector<unsigned int> & B,
	IloNum Ty, vector<unsigned int> & Y, IloNumVarArray & w);

// returns the max _{t >= U} ( \max (0, D(i) - t) - card(X)*u(t-z) )
template <class Dist>
IloNum deltaX(IloNum & t, IloNum z, unsigned int cardX,
	vector<IloNum> & Dxk, Dist & u);
template <class Dist>
IloNum deltaX(IloNum & t, IloNum z, unsigned int cardX,
	vector<IloNum> & Dxk, Dist & u, vector<IloNum> & etas);

// returns the max _{0 <= t <= Ly} ( \max (0, t - D(i)) -card(Y)v(t+s) )
template <class Dist>
IloNum deltaY(IloNum & t, IloNum s, unsigned int cardY,
	vector<IloNum> & Dyk, Dist & v);
template <class Dist>
IloNum deltaY(IloNum & t, IloNum s, unsigned int cardY,
   vector<IloNum> & Dyk, Dist & v, vector<IloNum> & etas);

// A = {i \in X: D^k(i) >= T^{k_x)_X }
Set getA(vector<IloNum> & Dx, Set & X, IloNum T);
// A = {i \in Y: D^k(i) <= T^{k_y}_Y }
Set getB(vector<IloNum> & Dy, Set & Y, IloNum T);

// returns a vector of all the doses of the elements of a given set
vector<IloNum> getDoses(Sparse_Matrix<IloInt>& vox, IloNumArray & w, vector<unsigned> & organSet);


/**** Iterative Methods ****/
// From The Cutting plane algorithm from Stochastic Programming Approaches for Radiation Therapy Planning
template <class dist>
bool cuttingPlaneMethod(IloEnv & env, Opt& solution, Sparse_Matrix<IloInt> & vox,
	Set & X, Set & Y, const IloNum& Ux, const IloNum& Ly, dist & u, dist & v);

template <class dist>
bool genCutPlaneMeth(IloEnv & env, Opt& solution, Sparse_Matrix<IloInt> & vox,
   map< string, Benchmark<dist> > & setAndBenchMarks);

template <class dist>
bool MIPMethod(IloEnv & env, Opt& solution, Sparse_Matrix<IloInt> & vox,
   map< string, Benchmark<dist> >& setAndBenchmarks);


/******** Objective Functions *****************/
IloExpr minDoses(IloEnv & env, IloNumVarArray & w, Sparse_Matrix<IloInt>& vox);
IloExpr minBeamletStr(IloEnv & env, IloNumVarArray & w, Sparse_Matrix<IloInt> & vox);

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

template <class Dist>
IloNum deltaX(IloNum & t, IloNum z, unsigned int cardX,
	vector<IloNum> & Dxk, Dist & u, vector<IloNum> & etas)
{
	IloNum max = -FLT_MAX;
	
	for(unsigned int it=0;it<etas.size();++it)
	{
		IloNum val=0;
		for(unsigned int i=0;i<Dxk.size();++i)
		{
			val += MAX( 0.f, Dxk[i] - etas[it]);
		}

		val -= IloNum(cardX)*u.intToInf(etas[it]-z);

		if(val >= max){
			max = val;
			t = etas[it];
		}
	}

	return max;
}

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

template <class Dist>
IloNum deltaY(IloNum & t, IloNum s, unsigned int cardY,
	vector<IloNum> & Dyk, Dist & v, vector<IloNum> & etas)
{
	IloNum max = -FLT_MAX;
	
	for(unsigned int it=0;it<etas.size();++it)
	{
		IloNum val=0;
		for(unsigned int i=0;i<Dyk.size();++i)
		{
			val += MAX( 0.f, etas[it] - Dyk[i]);
		}

		val -= IloNum(cardY)*v.intTo(etas[it]+s);

		if(val >= max){
			max = val;
			t = etas[it];
		}
	}

	return max;
}

template <class dist>
bool cuttingPlaneMethod(IloEnv & env, Opt& solution, Sparse_Matrix<IloInt> & vox,
	Set & X, Set & Y, const IloNum& Ux, const IloNum& Ly, dist & u, dist & v)
{
	// booleans for solutions
	bool solved = false;

	IloInt n, m;
	m = vox.m;
	n = vox.n;

	// decision variable
	try{
      IloNumVarArray w(env, n, 0, DBL_MAX);

		/******* Step 0 *********/
		unsigned int k, kx, ky;
		k = kx = ky = 1;
			
		while(!solved)
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

			IloCplex cplex(model);
			cplex.setOut(env.getNullStream());		// silent
			cplex.setParam(IloCplex::WorkMem, 7000); // add memory
			//cplex.setParam(IloCplex::MemoryEmphasis, 1); // save memory
			//cplex.setParam(IloCplex::Threads, 1);  // set thread count

			cout << "solving" << endl;
			while(!solved)
			{
				cout << "ITERATION " << k << endl;
		
				if( !cplex.solve() ){
					env.error() << "Failed to optimize LP." << endl;
               if(!cplex.isPrimalFeasible()){
                  env.out() << "Problem is not feasible" << endl;
               }
					throw(-1);
				}

				// Get solution
				IloNum min = cplex.getObjValue();
				IloNumArray vals(env);
				cplex.getValues(vals, w);

				// calculates the doses at the current solution
				vector<IloNum> Dx, Dy;
            Dx = getDoses(vox, vals, X);
            Dy = getDoses(vox, vals, Y);

				/****** Step 2 ******/
				IloNum dx, dy, Txk, Tyk;
				dx = deltaX(Txk, 0., unsigned(X.size()), Dx, u);
				dy = deltaY(Tyk, 0., unsigned(Y.size()), Dy, v);
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
				if(dx > 0.){
					++kx;
					model.add( twelveLHS(env, vox, getA(Dx,X,Txk), Txk, X, w) <=
                  IloNum(X.size())*u.intToInf(Txk) );
				}

				if(dy > 0.){
					++ky;
					model.add( thirteenLHS(env, vox, getB(Dy,Y,Tyk), Tyk, Y, w) <=
                  IloNum(Y.size())*v.intTo(Tyk) );
				}

				if(k == MAX_ITS){
					cout << "Reached max iterations " << endl;
					break;
				}

				++k;
			} // adding cuts
		}
	} // try
	catch(IloException& e){
		cout << "Concert exception caught: " << e << endl;
		return false;
	}
	catch(...){
		cout << "Unknown exception caught" << endl;
		return false;
	}
	return false; // never should reach here
}

template <class dist>
bool genCutPlaneMeth(IloEnv & env, Opt& solution, Sparse_Matrix<IloInt> & vox,
   map< string, Benchmark<dist> > & setAndBenchMarks)
{
   double start, end;
   start = omp_get_wtime();
	// booleans for solutions
	bool solved = false;

	IloInt n, m;
	m = vox.m;
	n = vox.n;
   unsigned constraint_count=0;

	// decision variable
	try{
		IloNumVarArray w(env, n, 0, DBL_MAX);

		/******* Step 0 *********/
		unsigned int k;
      k = 1;

		while(!solved)
		{
			/******* Step 1 *********/
			IloModel model(env);
			// objective function
			IloExpr obj = IloSum(w);
			// add the objective function
			model.add( IloMinimize( env, obj) );
			// begin to add constraints
         for(map<string, Benchmark<dist>>::iterator it=setAndBenchMarks.begin();
            it!=setAndBenchMarks.end();++it)
         {
            if(it->second.upetas.size() != 0){
               model.add( twelveLHS(env, vox, it->second.region, *(it->second.upetas.begin()), it->second.region, w) <=
                  IloNum(it->second.region.size())*(it->second.UpperDist.intToInf(*it->second.upetas.begin())) );
               ++constraint_count;
            }
            if(it->second.lowetas.size() != 0){
               model.add( thirteenLHS(env, vox, it->second.region, *(it->second.lowetas.rbegin()), it->second.region, w) <=
                  IloNum(it->second.region.size())*(it->second.LowerDist.intTo(*it->second.lowetas.rbegin())) );
               ++constraint_count;
            }
         }

         IloCplex cplex(model);
         cplex.setOut(env.getNullStream());		// silent
			cplex.setParam(IloCplex::WorkMem, 7000); // add memory
         cplex.setParam(IloCplex::RootAlg, IloCplex::Dual);
			//cplex.setParam(IloCplex::MemoryEmphasis, 1); // save memory
			//cplex.setParam(IloCplex::Threads, 1);  // set thread count

         // initilze for stuff inside the loop      
			IloNumArray vals(env);
         vector< vector<IloNum> > doses(setAndBenchMarks.size());
         vector< Delta > deltas(doses.size());
         map< string, Benchmark<dist>>::iterator bench_it;
			while(!solved)
			{
            if(k%100 == 0 || k == 1){
				   cout << "ITERATION " << k << endl;
               cout << cplex.getNrows() << " constraints " << endl;
               cout << "Memory usage " << env.getMemoryUsage()/(1024*1024) << endl;
               cout << "Extractables " << env.getMaxId() << endl;
            }
		
				if( !cplex.solve() ){
					env.error() << "Failed to optimize LP." << endl;
               if(!cplex.isPrimalFeasible()){
                  env.out() << "Problem is not feasible" << endl;
               }
					throw(-1);
				}

				// Get solution
				cplex.getValues(vals, w);
            IloNum min = cplex.getObjValue();
				
            // calculates the doses at the current solution
            bench_it=setAndBenchMarks.begin();
            for(vector< vector<IloNum> >::iterator v_it=doses.begin();
               v_it!=doses.end();++v_it)
            {
               *v_it = getDoses(vox, vals, bench_it->second.region);
               ++bench_it;
            }

				/****** Step 2 ******/         
            bench_it = setAndBenchMarks.begin();
            for(unsigned i=0;i<deltas.size();++i)
            {
               // calculate deltas for constraints
               deltas[i].upper = deltaX(deltas[i].t_upper, 0.,
                  unsigned(bench_it->second.region.size()),
                  doses[i], bench_it->second.UpperDist,
                  bench_it->second.upetas);
               deltas[i].lower = deltaY(deltas[i].t_lower, 0.,
                  unsigned(bench_it->second.region.size()),
                  doses[i], bench_it->second.LowerDist,
                  bench_it->second.lowetas);
               ++bench_it;
            }
            if(k%100 == 0){
               cout << "Actual Constraint values" << endl;
               for(unsigned i=0;i<deltas.size();++i){
                  cout << deltas[i].upper << " ";
               } cout << endl;
               for(unsigned i=0;i<deltas.size();++i){
                  cout << deltas[i].lower << " ";
               } cout << endl;
            }

				// check to see if solution is optimal
            IloNum max_exceed=-100.;
            for(unsigned i=0;i<deltas.size();++i)
            {
               max_exceed = MAX(max_exceed, deltas[i].upper);
               max_exceed = MAX(max_exceed, deltas[i].lower);
            }

				if(max_exceed <= EPSILON){
					cout << "\n\n\n" << endl;
					cout << "****************************************" << endl;
					cout << "A solution was found!" << endl;
					cout << k << " iterations" << endl;

					solution.Min = cplex.getObjValue();
					cplex.getValues( solution.ArgMin, w );
               end = omp_get_wtime();
               solution.RunTimeSeconds = end - start;

					return true;
				}
            
            // add cuts
            bench_it = setAndBenchMarks.begin();
            for(unsigned i=0;i<deltas.size();++i)
            {
               if(deltas[i].upper > 0.)
               {
                  IloExpr temp_expr(env);
                  twelveLHSRef(env, temp_expr, vox,
                     getA(doses[i], bench_it->second.region, deltas[i].t_upper),
                     deltas[i].t_upper, bench_it->second.region, w);
                  model.add( temp_expr <=
                     IloNum(bench_it->second.region.size())*
                     bench_it->second.UpperDist.intToInf(deltas[i].t_upper) ) ;
                  temp_expr.end();
                  ++constraint_count;
               }

               if(deltas[i].lower > 0.)
               {
                  IloExpr temp_expr(env);
                  thirteenLHSRef(env, temp_expr, vox,
                     getB(doses[i], bench_it->second.region, deltas[i].t_lower),
                     deltas[i].t_lower, bench_it->second.region, w);
                  model.add( temp_expr  <=
                     IloNum(bench_it->second.region.size())*
                     bench_it->second.LowerDist.intTo(deltas[i].t_lower) );
                  temp_expr.end();
                  ++constraint_count;
               }
               ++bench_it;
            }
            
				if(k == MAX_ITS){
					cout << "Reached max iterations " << endl;
					solution.Min = min;
					solution.ArgMin = vals;
               cplex.exportModel("save.lp");
               cout << "Objective min is " << min << endl;
               return false;
				}

				++k;
			} // adding cuts
		}
	} // try
	catch(IloException& e){
		cout << "Concert exception caught: " << e << endl;
		return false;
	}
	catch(...){
		cout << "Unknown exception caught" << endl;
		return false;
	}
	return false; // never should reach here
}

template <class dist>
bool MIPMethod(IloEnv & env, Opt& solution, Sparse_Matrix<IloInt> & vox,
   map< string, Benchmark<dist> >& setAndBenchmarks)
{
   double start, end;
   start = omp_get_wtime();
   try
   {
      IloModel model(env);

      IloNumVarArray w(env, vox.n, 0., DBL_MAX);
      model.add( IloMinimize(env, IloSum(w) ) );

      // add constraints
      for(map<string, Benchmark<dist> >::iterator b_it=setAndBenchmarks.begin();
         b_it!=setAndBenchmarks.end();++b_it)
      {
         for(unsigned i=0;i<b_it->second.lowetas.size();++i)
         {
            IloExpr expr(env);
            for(unsigned j=0;j<b_it->second.region.size();++j){
               expr += IloMax(0., b_it->second.lowetas[i] - D(b_it->second.region[j], vox, w, env));
            }
            expr -= IloNum(b_it->second.region.size())*b_it->second.LowerDist.intTo(
               b_it->second.lowetas[i]);
            model.add( expr <= EPSILON);
         }

         for(unsigned i=0;i<b_it->second.upetas.size();++i)
         {
            IloExpr expr(env);
            for(unsigned j=0;j<b_it->second.region.size();++j)
            {
               expr += IloMax(0., D(b_it->second.region[j], vox, w, env) - b_it->second.upetas[i]);
            }
            expr -= IloNum(b_it->second.region.size())*b_it->second.UpperDist.intToInf(
               b_it->second.upetas[i]);
            model.add( expr <= EPSILON);
         }
      }

      IloCplex cplex(model);

      if( !cplex.solve() )
      {
         if( !cplex.isPrimalFeasible() ){
            cout << "Problem is not feasible" << endl;
         }
         env.error() << "unable to solve" << endl;
         throw(-1);
      }

      solution.Min = cplex.getObjValue();
      cplex.getValues(w, solution.ArgMin );
      end = omp_get_wtime();
       
      solution.RunTimeSeconds = end - start;

      return true;

   }
   catch(IloException& e){
		cout << "Concert exception caught: " << e << endl;
		return false;
	}
	catch(...){
		cout << "Unknown exception caught" << endl;
		return false;
	}

	return false; // never should reach here
}

#endif // OPTIMIZE_H