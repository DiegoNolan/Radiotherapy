#pragma once

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <fstream>
#include <string>
#include <iostream>

using std::cout;
using std::endl;
using std::string;

typedef boost::numeric::ublas::compressed_matrix<IloInt>::iterator1 it1_t;
typedef boost::numeric::ublas::compressed_matrix<IloInt>::iterator2 it2_t;

// wrapper for sparse matrix

class Vox : public boost::numeric::ublas::compressed_matrix<IloInt>
{
public:
   Vox(unsigned voxels=0, unsigned beamlets=0,
      unsigned long max_non_zeroes=0);
   Vox(unsigned int voxels, unsigned int beamlets,
      vector<IloInt> non_zeroes, vector<unsigned> ii, vector<unsigned> jj);
   ~Vox(){}
   // write in the following form
   // rows columns
   // 1st non-zero val 2nd non-zero val ...
   // 1st non-zero row 2nd non-zero row ...
   // 1st non-zero col 2nd non-zero col ...
   void write(string fname);
   // reads in file of same type
   void read(string fname);
private:

};
