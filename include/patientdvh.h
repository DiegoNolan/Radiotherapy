
#ifndef PATIENTDVH_H
#define PATIENTDVH_H

#include <iostream>

#include "fileio.h"
#include "histogram.h"

using std::cout;
using std::endl;

class PatientDVH
{
public:
   PatientDVH();
   // reads the DVH_Tabular Format file
   PatientDVH(string filename);
   ~PatientDVH();
   vector<Histogram> structures;
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
};

#endif // PATIENTDVH_H
