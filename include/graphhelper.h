#pragma once

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include <vector>

#include "sparse_matrix.h"
#include "graph.h"
#include "disthelper.h"
#include "benchmark.h"

using std::vector;

template <class dist>
void makeGraphs(vector<Graph*> & graphs, Sparse_Matrix<IloInt> & vox,
   IloNumArray & w, vector<Benchmark<dist>> & setAndBenchs); 


template <class dist>
void makeGraphs(vector<Graph*> & graphs, Sparse_Matrix<IloInt> & vox,
   IloNumArray & w, vector<Benchmark<dist>> & setAndBenchs)
{
   vector<DistHelper> distHelpers(setAndBenchs.size());
   for(unsigned i=0;i<distHelpers.size();++i)
   {
      distHelpers[i] = DistHelper(&vox, &setAndBenchs[i].region, &w);
   }

   graphs.clear();
   graphs.resize(setAndBenchs.size()*3 );
   for(unsigned i=0;i<setAndBenchs.size();++i)
   {
      // cdf
      graphs[i*3] = new Graph("OAR", 600, 500, -1., 100., -.2, 1.5);
      graphs[i*3]->addplot(&setAndBenchs[i].LowerDist, &UniformDist::cdf);
      graphs[i*3]->addplot(&setAndBenchs[i].UpperDist, &UniformDist::cdf);
      graphs[i*3]->setColor(0,255,0);
      graphs[i*3]->addplot(&distHelpers[i], &DistHelper::dist);
      // F2
      graphs[i*3+1] = new Graph("F2", 600, 500, -1., 100., -5., 50.);
      graphs[i*3+1]->addplot(&setAndBenchs[i].LowerDist, &UniformDist::intTo);
      graphs[i*3+1]->setColor(0,255,0);
      graphs[i*3+1]->addplot(&distHelpers[i], &DistHelper::intTo);
      // H2
      graphs[i*3+2] = new Graph("H2", 600, 500, -1., 100., -5., 50.);
      graphs[i*3+2]->addplot(&setAndBenchs[i].UpperDist, &UniformDist::intToInf);
      graphs[i*3+2]->setColor(0,255,0);
      graphs[i*3+2]->addplot(&distHelpers[i], &DistHelper::intToInf);
   }
}