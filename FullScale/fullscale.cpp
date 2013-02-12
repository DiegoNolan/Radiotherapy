
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
      UniformDist(-10., -9.), UniformDist(70., 130.), -10., -9.,
      0, 70., 130., 10); /*
   setAndBenches["L PAROTID"] = Benchmark<UniformDist>(
      UniformDist(-10., -9.), UniformDist(60., 70.), -10., -9.,
      0, 60., 70., 10);
   setAndBenches["pituitary"] = Benchmark<UniformDist>(
      UniformDist(-10., -9.), UniformDist(33., 40.), -10., -9.,
      0, 33., 40., 10); */
   setAndBenches["PTV 1"] = Benchmark<UniformDist>(
      UniformDist(10., 70.), UniformDist(80., 100.), 10., 70.,
      10, 80., 100., 0);/*
   setAndBenches["R PAROTID"] = Benchmark<UniformDist>(
      UniformDist(-10., -9.), UniformDist(50., 60.), -10., -9.,
      0, 50., 60., 10); */

   for(std::map<string, Benchmark<UniformDist> >::iterator it=
      setAndBenches.begin();it!=setAndBenches.end();++it)
   {
      readSet(it->second.region, "../data/" + it->first + ".set");
      cout << it->second.region.size() << endl;
   }
   
   IloEnv env;
   Opt solution(env);

   if( !genCutPlaneMeth(env, solution, vox, setAndBenches) ){
      cout << "Could not solve" << endl;
      std::cin.ignore();
      return EXIT_FAILURE;
   }

   vector<Graph*> graphs;

   makeGraphs(graphs, vox, solution.ArgMin, setAndBenches);

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
