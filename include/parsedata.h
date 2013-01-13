#pragma once

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include <set>
#include <algorithm>
#include <iterator>
#include <string>
#include <sstream>

#include "set.h"
#include "sparsematrix.h"	// format 1
#include "sparse_matrix.h"  // Yale format
#include "voxelmapping.h"
#include "beamLetmapping.h"

// shortest line is atleast longer than this amount of chars
#define MIN_LINE 10
// how much to resize vectors when need more space
#define RESIZE_AMOUNT 10000
// how close floats have to be to each other to be considered equal
#define FLT_TOL 0.0001f
// how many lines the x, y, z data is
#define XYZ_LENGTH 21



// fills the matrix with the dose data and the sets with the organ data
// and prints a bunch of stats about the data
void parseOrganData(Sparse_Matrix<IloInt> & vox, vector<std::set<unsigned> > & organSets,
   VoxelMapping & voxelmap, vector<string> fnames, string vox_name);

// returns true if the charactor is a number
bool isNum(char & ch);

// returns true if charactor is a number, '-' or ' '
bool isDataLine(char & firstchar);

// returns the number of voxels in the data file
unsigned int numVoxels(char * buffer, unsigned long bufferSize);

// gets the amount of beamlets in the data file
vector<vec2> getBeamletCounts(char * buffer, unsigned long bufferSize);

// returns the total beamlets from the vector
unsigned int getTotalBeamlets(const vector<vec2>& beamletCounts);

// returns the sparse matrix of all the doses caused on voxel i by beamlet j
Sparse_Matrix<IloInt> getVoxMatrix(char * buffer, unsigned long bufferSize,
	vector<vec2> beamletCounts);

// returns the set of voxels that belong to the organ
std::set<unsigned> getOrganSet(char * buffer, unsigned int bufferSize);

// returns the voxel mapping from the file
VoxelMapping getVoxelMapping(char * buffer, unsigned bufferSize);

// checks for equality of floats inside a certain tolerance
bool isEqual(float one, float two, float tol);

// returns true if was able to open file
bool fileExists(const std::string& filename);

// do files exist
bool allFiles(string vox_name, vector<string> organ_names);