#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <iostream>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::cout;
using std::endl;

typedef struct
{
   double relDose;
   double dose;
   double ratioTotStruct;
} Histogram_Triplet;


class Histogram
{
public:
   Histogram();
   Histogram(string name);
   ~Histogram();
   void setData(const vector<Histogram_Triplet> & dat);
   void operator= (const Histogram & hist);
   string getName(){return structName;}
   void print();
   // returns a linearally interpolated value
   double getValFromRel(double rel_dose);
private:
   string structName;
   vector<Histogram_Triplet> data;
};

#endif // HISTOGRAM_H
