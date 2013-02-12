#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include <iostream>
#include <cmath>

#include "sparse_matrix.h"
#include "set.h"
#include "benchmark.h"
#include "distribution.h"
#include "optimize.h"
#include "graph.h"
#include "graphhelper.h"
#include <string>
#include <map>

using std::map;
using std::string;
using std::cout;
using std::endl;

int main()
{
   IloEnv env;
   try
   {
      IloModel model(env);

      IloCplex cplex(env);
   }
	catch(IloException& e){
		cout << "Concert exception caught: " << e << endl;
      cin.ignore();
		return false;
	}
   catch(...)
   {
      env.error() << "error " << endl;
      cin.ignore();
      return -1;
   }

   std::cin.ignore();
   env.end();

   std::cin.ignore();
   return 0;
}
