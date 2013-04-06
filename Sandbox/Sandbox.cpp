#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include <iostream>
#include <cmath>
#include <fstream>

#include "sparse_matrix.h"
#include "set.h"
#include "benchmark.h"
#include "distribution.h"
#include "optimize.h"
#include "graph.h"
#include "graphhelper.h"
#include "patientdvh.h"
#include <string>
#include <map>

using std::map;
using std::string;
using std::cout;
using std::endl;

int main()
{
   PatientDVH pdata("../data/DVH _Tabular Format");
   
   pdata.genGraph();

   while(1)
   {
      pdata.displayGraph();
   }
  
   std::cin.ignore();
   return 0;
}
