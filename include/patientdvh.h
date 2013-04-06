
#ifndef PATIENTDVH_H
#define PATIENTDVH_H

#include <iostream>

#include "fileio.h"
#include "histogram.h"
#include "graph.h"

using std::cout;
using std::endl;

const int color_array[][3] =
{
   { 250, 0, 0 },
   { 200, 50, 0 },
   { 150, 100, 0 },
   { 100, 150, 0 },
   { 50, 200, 0 },
   { 0, 250, 0 },
   { 0, 200, 50 },
   { 0, 150, 100 },
   { 0, 100, 150 },
   { 0, 50, 200 },
   { 0, 0, 250 },
   { 50, 0, 200 },
   { 100, 0, 150 },
   { 150, 0, 100 },
   { 200, 0, 50 },
   { 250, 0, 0 }
};

class PatientDVH
{
public:
   PatientDVH();
   // reads the DVH_Tabular Format file
   PatientDVH(string filename);
   ~PatientDVH();
   vector<Histogram> structures;
   void genGraph();
   void displayGraph();
private:
   void getPatientName(string & f_contents);
   string getStructureName(string & f_contents, size_t & pos);
   size_t findFirstDataLine(string & f_contents, size_t start_pos);
   // returns true if data line
   bool isDataLine(string & f_contents, size_t pos);
   // gets the next number and sets the pos to the element right after that number
   // assumes all positve numbers
   double getNextNumber(string & f_contents, size_t & pos);
   string patientName;
   Graph * graphptr;
};

#endif // PATIENTDVH_H
