
#include <iostream>

#include "graph.h"
#include "optimize.h"
#include "graphhelper.h"

using std::cout;
using std::endl;

int main()
{
   vector<string> fnames;
   string vox_name = "run1";
   Sparse_Matrix<IloInt> vox;
   vox.read("../data/" + vox_name + ".vox");

   // create benchmarks and sets
   map<string, Benchmark<UniformDist> > setAndBenches;
   setAndBenches["Spinalcord+0.5"] = Benchmark<UniformDist>(
      UniformDist(-10., -9.), UniformDist(30., 120.), -10., -9.,
      0, 30., 120., 10);
   setAndBenches["L PAROTID"] = Benchmark<UniformDist>(
      UniformDist(-10., -9.), UniformDist(30., 120.), -10., -9.,
      0, 30., 120., 10);
   setAndBenches["pituitary"] = Benchmark<UniformDist>(
      UniformDist(-10., -9.), UniformDist(30., 120), -10., -9.,
      0, 30., 120., 10);
   setAndBenches["PTV 1"] = Benchmark<UniformDist>(
      UniformDist(10., 100.), UniformDist(100., 200.), 10., 100.,
      10, 100., 200., 0);
   setAndBenches["R PAROTID"] = Benchmark<UniformDist>(
      UniformDist(-10., -9.), UniformDist(30., 120.), -10., -9.,
      0, 30., 120., 10);

   for(std::map<string, Benchmark<UniformDist> >::iterator it=
      setAndBenches.begin();it!=setAndBenches.end();++it)
   {
      readSet(it->second.region, "../data/" + it->first + ".set");
      cout << it->second.region.size() << endl;
   }

  /* unsigned one, two, three, four, five;
   one = two = three = four = five = 0;
   unsigned min = 100000;
   unsigned max = 0;
   unsigned sum = 0;
   double mean;
   unsigned temp;
   for(unsigned i=0;i<setAndBenches["PTV 1"].region.size();++i)
   {
      temp = vox.getNumNonZeros( setAndBenches["PTV 1"].region[i] );
      if(temp < min)
      {
         min = temp;
      }
      if(temp > max)
      {
         max = temp;
      }

      switch(temp)
      {
      case 1:
         ++one;
         break;
      case 2:
         ++two;
         break;
      case 3:
         ++three;
         break;
      case 4:
         ++four;
         break;
      case 5:
         ++five;
         break;

      }

      sum += temp;
   }

   mean = double(sum)/double(setAndBenches["PTV 1"].region.size());
   cout << "For PTV" << endl;
   cout << "Min: " << min << " Max: " << max << " Mean: " << mean << endl;
   cout << "1: " << one << " 2: " << two << " 3: " << three << " 4: " << four << " 5: " << five << endl;
   
   std::cin.ignore();*/
   
   IloEnv env;
   Opt solution(env);

   if( !genCutPlaneMeth(env, solution, vox, setAndBenches) ){
      cout << "Could not solve" << endl;
      std::cin.ignore();
      return EXIT_FAILURE;
   }

   solution.write("../data/CP_500");

   cout << endl << "Method ran in " << solution.RunTimeSeconds << " seconds" << endl;

   vector<Graph*> graphs;

   makeGraphs(graphs, vox, solution.ArgMin, setAndBenches);

   cout << "displaying graphs" << endl;

   while(1)
   {
      for(unsigned i=0;i<graphs.size();++i){
         graphs[i]->update();
      }
   }
   
   // deallocate graphs
   for(unsigned i=0;i<graphs.size();++i){
      delete graphs[i];
   }

   
   std::cin.ignore();
   return 0;
}


