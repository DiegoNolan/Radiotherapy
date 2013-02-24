#include "histogram.h"

Histogram::Histogram()
{

}

Histogram::Histogram(string name)
{
   structName = name;
}

Histogram::~Histogram()
{

}

void Histogram::setData(const vector<Histogram_Triplet> & dat)
{
   data = dat;
}

void Histogram::operator= (const Histogram & hist)
{
   structName = hist.structName;
   data = hist.data;
}

void Histogram::print()
{
   cout << "Structure Name : " << structName << endl;
   for(unsigned i=0;i<data.size();++i)
   {
      cout << data[i].relDose << '\t' << data[i].dose << '\t' << data[i].ratioTotStruct << endl;
   }
}
