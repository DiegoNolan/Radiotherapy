#pragma once

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include <vector>
#include <map>
#include <string>

#include "sparse_matrix.h"
#include "graph.h"
#include "disthelper.h"
#include "benchmark.h"

using std::vector;
using std::map;
using std::string;

template <class dist>
void makeGraphs(vector<Graph*> & graphs, Sparse_Matrix<IloInt> & vox,
   IloNumArray & w, map< string, Benchmark<dist>> & setAndBenchs); 


template <class dist>
void makeGraphs(vector<Graph*> & graphs, Sparse_Matrix<IloInt> & vox,
   IloNumArray & w, map< string, Benchmark<dist>> & setAndBenchs)
{
   map< string, Benchmark<dist>>::iterator b_it = setAndBenchs.begin();
   vector<DistHelper> distHelpers(setAndBenchs.size());
   for(unsigned i=0;i<distHelpers.size();++i)
   {
      distHelpers[i] = DistHelper(&vox, &b_it->second.region, &w);
      ++b_it;
   }

   graphs.clear();
   graphs.resize(setAndBenchs.size()*3 );
   b_it = setAndBenchs.begin();
   for(unsigned i=0;i<setAndBenchs.size();++i)
   {
      // cdf
      graphs[i*3] = new Graph(b_it->first+" cdf", 600, 500, -1., 100., -.2, 1.5);
      graphs[i*3]->addplot(&b_it->second.LowerDist, &dist::cdf);
      graphs[i*3]->addplot(&b_it->second.UpperDist, &dist::cdf);
      graphs[i*3]->setColor(0,255,0);
      graphs[i*3]->addplot(&distHelpers[i], &DistHelper::dist);
      // F2
      graphs[i*3+1] = new Graph(b_it->first+" F2", 600, 500, -1., 100., -5., 50.);
      graphs[i*3+1]->addplot(&b_it->second.LowerDist, &dist::intTo);
      graphs[i*3+1]->setColor(0,255,0);
      graphs[i*3+1]->addplot(&distHelpers[i], &DistHelper::intTo);
      // H2
      graphs[i*3+2] = new Graph(b_it->first+" H2", 600, 500, -1., 100., -5., 50.);
      graphs[i*3+2]->addplot(&b_it->second.UpperDist, &dist::intToInf);
      graphs[i*3+2]->setColor(0,255,0);
      graphs[i*3+2]->addplot(&distHelpers[i], &DistHelper::intToInf);
      ++b_it;
   }
}