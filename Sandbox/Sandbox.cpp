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

   for(unsigned i=0;i<pdata.structures.size();++i)
   {
      if(pdata.structures[i].getName() == "BRAINSTEM")
      {
         pdata.structures[i-1].print();
      }
   }
  
   std::cin.ignore();
   return 0;
}
