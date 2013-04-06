#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using std::cout;
using std::endl;
using std::vector;
using std::string;


template < class T >
class Sparse_Matrix
{
public:
	Sparse_Matrix(){}	
	~Sparse_Matrix(){}
	void read(string fname);
	void write(string fname);
   // copy operator from a vector of vectors, forming a dense matrix
	void operator= (vector<vector<T> > & mat);
   // return true if there is a non-zero element in a row
   bool rowHasElems(unsigned i)
   {
      if( ia[i] == ia[i+1] ){
         return false;
      }

      return true;
   }

   // returns the value
	T operator() (unsigned int i, unsigned int j)
	{
		if(ia[i] == ia[i+1]){
			return 0;
		}

		for(unsigned int k = ia[i];k<ia[i+1];++k)
      {
			if(ja[k] == j){
				return a[k];
			}
			else if(ja[k] > j){
				return 0;
			}
		}
		return 0;
	}

   // returns true if the matrices are not the same
   bool operator!= (Sparse_Matrix<T> arg)
   {
      if(this->a.size() != arg.a.size() ||
         this->ia.size() != arg.ia.size() ||
         this->ja.size() != arg.ja.size())
      {
         return true;
      }

      if(this->m != arg.m ||
         this->n != arg.n){
         return true;
      }

      for(unsigned i=0;i<this->a.size();++i){
         if( this->a[i] != arg.a[i] ){
            return true;
         }
      }

      for(unsigned i=0;i<this->ia.size();++i){
         if( this->ia[i] != arg.ia[i] ){
            return true;
         }
      }

      for(unsigned i=0;i<this->ja.size();++i){
         if( this->ja[i] != arg.ja[i] ){
            return true;
         }
      }

      return false;
   }

   // copy operator
   void operator= (Sparse_Matrix<T> arg)
   {
      this->a = arg.a;
      this->ia = arg.ia;
      this->ja = arg.ja;
      this->m = arg.m;
      this->n = arg.n;
   }

   // m is rows, n is columns
	unsigned int m, n;

   // prints the vectors for debugging
	void debugPrint()
	{
		for(unsigned int i=0;i<a.size();++i)
		{
			cout << a[i] << " ";
		}
		cout << endl;

		for(unsigned int i=0;i<ia.size();++i)
		{
			cout << ia[i] << " ";
		}
		cout << endl;

		for(unsigned int i=0;i<ja.size();++i)
		{
			cout << ja[i] << " ";
		}
		cout << endl;
		
	}
   unsigned getNumNonZeros(unsigned row)
   {
      return ia[row+1]-ia[row];
   }

	vector<T> a;				// non zero elements
	vector<unsigned int> ia;	// index of a for all elements in i row
	vector<unsigned int> ja;	// j for a
	
private:
	

};

template <class T>
void Sparse_Matrix<T>::write(string fname)
{
	std::ofstream output(fname);

	if(!output.is_open()){
		cout << "Error opening " << fname << endl;
	}

	output << m << " " << n << "\n";

	for(unsigned int i=0;i<a.size()-1;++i)
	{
		output << a[i] << " ";
	}
	output << a[a.size()-1] << "\n";

	for(unsigned int i=0;i<ia.size()-1;++i)
	{
		output << ia[i] << " ";
	}
	output << ia[ia.size()-1] << "\n";

	for(unsigned int i=0;i<ja.size()-1;++i)
	{
		output << ja[i] << " ";
	}
	output << ja[ja.size()-1] << "\n";
	
	output.close();
}

template <class T>
void Sparse_Matrix<T>::read(string fname)
{
	std::ifstream input(fname);

	if(!input.is_open()){
		cout << "Error opening " << fname << endl;
	}
	
	string linestring;
	// get m and n
	std::getline(input, linestring);
	std::stringstream ss;
	ss << linestring;
	ss >> m; ss >> n;

	ss.clear();
	// a
	std::getline(input, linestring);
	ss << linestring;
	unsigned int size = 0;
	unsigned int temp;
	while(!ss.eof()){
		ss >> temp;
		++size;
	}
	a.resize(size);
	ss.clear();
	ss << linestring;
	size = 0;
	while(!ss.eof()){
		ss >> a[size];
		++size;
	}
	
	// i
	ss.clear();
	std::getline(input, linestring);
	ss << linestring;
	size = 0;
	while(!ss.eof()){
		ss >> temp;
		++size;
	}
	ia.resize(size);
	ss.clear();
	ss << linestring;
	size = 0;
	while(!ss.eof()){
		ss >> ia[size];
		++size;
	}

	// j
	ss.clear();
	std::getline(input, linestring);
	ss << linestring;
	size = 0;
	while(!ss.eof()){
		ss >> temp;
		++size;
	}
	ja.resize(size);
	ss.clear();
	ss << linestring;
	size = 0;
	while(!ss.eof()){
		ss >> ja[size];
		++size;
	}

	input.close();

	// check for validity
	if( ia.size() != m+1){
		cout << "Error in vox file" << endl;
	}

}

template < class T >
void Sparse_Matrix<T>::operator= (vector<vector<T> >& mat)
{
	m = unsigned(mat.size());
	n = unsigned(mat[0].size());
	a.clear();
	ia.clear();
	ja.clear();

	ia.push_back(0);
	for(unsigned int i=0;i<mat.size();++i)
	{
		for(unsigned int j=0;j<mat[i].size();++j)
		{
			if(mat[i][j] != 0)
			{
				a.push_back(mat[i][j]);
				ja.push_back(j);
			}
		}
		ia.push_back(unsigned(a.size()));
	}
}

/*
template <class T>
std::ostream& operator << (std::ostream& stream, const Sparse_Matrix<T> & matrix)
{
	for(unsigned int i=0;i<matrix.m;++i)
	{
		for(unsigned int j=0;j<matrix.n;++j)
		{
			stream << matrix(i, j) << " ";
		}
		stream << endl;
	}
	return stream;
}
*/

#endif // SPARSE_MATRIX_H
