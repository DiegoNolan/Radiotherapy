#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::vector;

class SparseMatrix
{
public:
	SparseMatrix(){}
	~SparseMatrix(){}
	bool write(string fname);
	void read(string fname);
	void sort();
	vector<short> a;	// The non zero elements
	vector<unsigned int> i;		// corresponding row
	vector<unsigned int> j;		// corresponding column
	unsigned int m, n;
};


// functions to aid in sorting

// flips the values of the indices in all vectors: a, i, j
void flip(SparseMatrix& mat, unsigned int first, unsigned int second);

unsigned int partitioni(SparseMatrix& mat, long left, long right, long pivotIndex);

unsigned int partitionj(SparseMatrix& mat, long left, long right, long pivotIndex);

void inPlaceSorti(SparseMatrix& mat, long left, long right);

void inPlaceSortj(SparseMatrix& mat, long left, long right);

// My laptop doesn't have enough memory, save for later
SparseMatrix sorti(SparseMatrix& mat);
