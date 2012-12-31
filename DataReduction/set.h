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

void writeSet(std::set<unsigned> & set, string fname);

void readSet(std::set<unsigned> & set, string fname);

unsigned int factorial(unsigned int n);

unsigned int choose(unsigned int n, unsigned int r);
