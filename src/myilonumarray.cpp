#include "myilonumarray.h"

MyIloNumArray::MyIloNumArray()
{

}

MyIloNumArray::~MyIloNumArray()
{

}

IloNum& MyIloNumArray::operator[] (unsigned int it)
{
	if(it >= data.size())
	{
		cout << "Error: out of range" << endl;
	}

	return data[it];
}