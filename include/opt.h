#pragma once

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include <string>

using std::string; 

class Opt
{
public:
	Opt(IloEnv & env);
	~Opt(){};
	IloNum Min;
	IloNumArray ArgMin;
	IloNumArray Duals;
   double RunTimeSeconds;
	void write(string fname);
};

