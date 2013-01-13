#include "sparsematrix.h"

bool SparseMatrix::write(string fname)
{
	std::ofstream output(fname.c_str());

	if(!output.is_open()){
		cout << "Error opening " << fname << endl;
		return false;
	}

	output << m << " "<< n << "\n";

	for(unsigned int ii=0;ii<a.size()-1;++ii)
	{
		output << a[ii] << " "<< i[ii] << " "<< j[ii] << "\n";
	}
	output << a[a.size()-1] << " " << i[i.size()-1] << " " << j[j.size()-1];

	output.close();
	return true;
}

void SparseMatrix::read(string fname)
{
	std::ifstream input(fname.c_str());

	if(!input.is_open()){
		cout << "Error opening " << fname << endl;
	}
	
	string linestring;
	std::getline(input,linestring);
	std::stringstream ss;
	ss << linestring;
	ss >> m;
	ss >> n;

	// get line count
	unsigned int count = 0;
	while(std::getline(input, linestring))
	{
		++count;
	}

	// allocate space
	a.resize(count);
	i.resize(count);
	j.resize(count);

	input.clear();
	input.seekg(0, std::ios_base::beg);

	std::getline(input, linestring);

	count = 0;
	// read data
	while(std::getline(input, linestring))
	{
		ss.clear();
		ss << linestring;
		ss >> a[count];
		ss >> i[count];
		ss >> j[count];
		++count;
	}
}

void SparseMatrix::sort()
{
	// sort the vox array
	inPlaceSorti(*this, 0, long(i.size()-1));

	// now we have to sort in j
	unsigned int ii, jj;
	for(ii=0;ii<i.size()-1;++ii)
	{
		for(jj=ii+1;jj<i.size();++jj)
		{
			if(i[ii] != i[jj]){
				--jj;
				break;
			}
		}

		// sort j's
		if( jj > ii){
			inPlaceSortj(*this, ii, jj);
		}
	}
}

void flip(SparseMatrix& mat, unsigned int first, unsigned int second)
{
	short a_store;
	unsigned int i_store, j_store;
	
	a_store = mat.a[first];
	i_store = mat.i[first];
	j_store = mat.j[first];

	mat.a[first] = mat.a[second];
	mat.i[first] = mat.i[second];
	mat.j[first] = mat.j[second];

	mat.a[second] = a_store;
	mat.i[second] = i_store;
	mat.j[second] = j_store;
}

unsigned int partitioni(SparseMatrix& mat, long left, long right, long pivotIndex)
{
	unsigned int pivotValue = mat.i[pivotIndex];
	flip(mat, pivotIndex, right);
	unsigned int storeInd = left;

	for(long i=left;i<right;++i)
	{
		if(mat.i[i] < pivotValue)
		{
			flip(mat, i, storeInd);
			++storeInd;
		}
	}

	flip(mat, storeInd, right);
	return storeInd;
}

unsigned int partitionj(SparseMatrix& mat, long left, long right, long pivotIndex)
{
	unsigned int pivotValue = mat.j[pivotIndex];
	flip(mat, pivotIndex, right);
	unsigned int storeInd = left;

	for(long i=left;i<right;++i)
	{
		if(mat.j[i] < pivotValue)
		{
			flip(mat, i, storeInd);
			++storeInd;
		}
	}

	flip(mat, storeInd, right);
	return storeInd;
}

void inPlaceSorti(SparseMatrix& mat, long left, long right)
{
	if(left < right)
	{
		unsigned int pivotIndex =  left+(right-left)/2;

		unsigned int pivotNewIndex = partitioni(mat, left, right, pivotIndex);

		inPlaceSorti(mat, left, pivotNewIndex-1);

		inPlaceSorti(mat, pivotNewIndex+1, right);
	}
}

void inPlaceSortj(SparseMatrix& mat, long left, long right)
{
	if(left < right)
	{
		unsigned int pivotIndex =  left+(right-left)/2;

		unsigned int pivotNewIndex = partitionj(mat, left, right, pivotIndex);

		inPlaceSortj(mat, left, pivotNewIndex-1);

		inPlaceSortj(mat, pivotNewIndex+1, right);
	}
}

// My laptop doesn't have enough memory, save for later
SparseMatrix sorti(SparseMatrix& mat)
{
	if(mat.a.size() <= 1){
		return mat;
	}
	// first count elements less than and greater than
	unsigned int pivotValue = mat.i[0];
	unsigned int numLessThan = 0;
	unsigned int numGreaterThan = 0;
	for(unsigned int i=1;i<mat.i.size();++i)
	{
		if(mat.i[i] < pivotValue){
			++numLessThan;
		}else{
			++numGreaterThan;
		}
	}

	// allocate space
	SparseMatrix lessThan;
	SparseMatrix greaterThan;

	lessThan.a.resize(numLessThan);
	lessThan.i.resize(numLessThan);
	lessThan.j.resize(numLessThan);

	greaterThan.a.resize(numGreaterThan);
	greaterThan.i.resize(numGreaterThan);
	greaterThan.j.resize(numGreaterThan);

	numLessThan = numGreaterThan = 0;
	for(unsigned int i=1;i<mat.i.size();++i)
	{
		if(mat.i[i] < pivotValue){
			lessThan.a[numLessThan] = mat.a[i];
			lessThan.i[numLessThan] = mat.i[i];
			lessThan.j[numLessThan] = mat.j[i];
			++numLessThan;
		}else{
			greaterThan.a[numGreaterThan] = mat.a[i];
			greaterThan.i[numGreaterThan] = mat.i[i];
			greaterThan.j[numGreaterThan] = mat.j[i];
			++numGreaterThan;
		}	
	}

	lessThan = sorti(lessThan);
	greaterThan = sorti(greaterThan);
	lessThan.a.push_back(mat.a[0]);
	lessThan.i.push_back(mat.i[0]);
	lessThan.j.push_back(mat.j[0]);
	
	lessThan.a.insert(lessThan.a.end(), greaterThan.a.begin(), greaterThan.a.end() );
	lessThan.i.insert(lessThan.i.end(), greaterThan.i.begin(), greaterThan.i.end() );
	lessThan.j.insert(lessThan.j.end(), greaterThan.j.begin(), greaterThan.j.end() );

	return lessThan;
}