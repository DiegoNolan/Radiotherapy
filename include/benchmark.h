#pragma once

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include <set>
#include <vector>

#include "set.h"

using std::vector;
using std::set;

// Class that contains the set and the benchmarks it must satisfy 

template <class T>
class Benchmark
{
public:
   // Upper distribution and lower distrubiton and the values to be checked at
   Benchmark(){}
   Benchmark(T & lowdist, T & updist, IloNum lowlow, IloNum highlow, unsigned low_n,
      IloNum lowup, IloNum highup, unsigned up_n);
   Benchmark(T & lowdist, T & updist, IloNum lowlow, IloNum highlow, unsigned low_n,
      IloNum lowup, IloNum highup, unsigned up_n, Set & reg);
   void operator=(Benchmark<T> & arg);
   ~Benchmark(){}
   T LowerDist, UpperDist;
   vector<IloNum> upetas, lowetas;
   Set region;
private:
};

template <class T>
Benchmark<T>::Benchmark(T & lowdist, T & updist, IloNum lowlow,
   IloNum highlow, unsigned low_n, IloNum lowup, IloNum highup, unsigned up_n)
{
   UpperDist = updist;
   LowerDist = lowdist;

   upetas = getRange(lowup, highup, up_n);
   lowetas = getRange(lowlow, highlow, low_n);
}

template <class T>
Benchmark<T>::Benchmark(T & lowdist, T & updist, IloNum lowlow, IloNum highlow, unsigned low_n,
   IloNum lowup, IloNum highup, unsigned up_n, Set & reg)
{
   UpperDist = updist;
   LowerDist = lowdist;

   upetas = getRange(lowup, highup, up_n);
   lowetas = getRange(lowlow, highlow, low_n);

   region = reg;
}

template <class T>
void Benchmark<T>::operator=(Benchmark<T> & arg)
{
   LowerDist = arg.LowerDist;
   UpperDist = arg.UpperDist;
   upetas = arg.upetas;
   lowetas = arg.lowetas;
   region = arg.region;
}



template <class T>
vector<T> getRange( T low, T high, unsigned n)
{
   if(!(high > low)){
      cout << "Error: the higher value is not greater than the lower value" << endl;
   }

   vector<T> temp(n);
   T spacing = (high-low)/(T(n)-1);

   for(unsigned i=0;i<temp.size();++i)
   {
      temp[i] = low + T(i)*spacing;   
   }

   return temp;
}



