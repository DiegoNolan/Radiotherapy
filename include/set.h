#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

using std::cout;
using std::endl;
using std::string;
using std::vector;

typedef vector<unsigned int> Set;

void writeSet(Set & set, string fname);
template <class T>
void writeSet(std::set<T> & set, string fname); 

void readSet(Set & set, string fname);
template <class T>
void readSet(std::set<T> & set, string fname);

Set mergeSets(Set & set1, Set & set2);

unsigned factorial(unsigned n);

unsigned choose(unsigned n, unsigned r);

template <class T>
void writeSet(std::set<T> & set, string fname)
{
	// write
	std::ofstream output(fname);

	if(!output.is_open()){
		cout << "Error opening " << fname << endl;
	}

   for(std::set<unsigned>::iterator it=set.begin();
      it!=(--set.end());++it)
   {
		output << *it << "\n";
	}
	output << *(--set.end());

	output.close();
}

template <class T>
void readSet(std::set<T> & set, string fname)
{
	std::ifstream input(fname);

	if(!input.is_open()){
		cout << "Error opening " << fname << endl;
	}

	string linestring;
	unsigned int count =0;
	// get line count
	while(std::getline(input, linestring))
	{
		++count;
	}

	// allocate space
	vector<T> temp(count);

	input.clear();
	input.seekg(0, std::ios_base::beg);

	count = 0;
	while(std::getline(input, linestring))
	{
		std::stringstream ss;
		ss << linestring;
		ss >> temp[count];
		++count;
	}

   set = std::set<unsigned>(temp.begin(),temp.end());
}

