#ifndef MYILONUMARRAY_H
#define MYILONUMARRAY_H

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN
#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

class MyIloNumArray
{
public:
	MyIloNumArray();
	~MyIloNumArray();
	IloNum& operator[] (unsigned int);
private:
	vector<IloNum> data;
};

#endif MYILONUMARRAY_H