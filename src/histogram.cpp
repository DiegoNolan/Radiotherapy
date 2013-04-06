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

double Histogram::getValFromRel(double rel_dose)
{
   if(rel_dose <= 0.)
   {
      return data[0].ratioTotStruct;
   }

   for(unsigned i=1;i<data.size();++i)
   {
      if(rel_dose < data[i].relDose)
      {
         double slope = (data[i].ratioTotStruct-data[i-1].ratioTotStruct)/
            (data[i].relDose-data[i-1].relDose);
         return slope*(rel_dose-data[i-1].relDose) + data[i-1].ratioTotStruct;
      }
   }

   return data[data.size()-1].ratioTotStruct;
}