#pragma once

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include <vector>
#include <iostream>
#include <string>
#include <fstream>

using std::cout;
using std::vector;
using std::string;

class Distribution
{
public:
	// Constructor for uniform
	Distribution(IloNum l, IloNum u);
	~Distribution(){}
	// returns the cdf of the distribution
	IloNum cdf(IloNum t);
	// returns the integral of the cdf from negative infinity to the argument
	IloNum intTo(IloNum T);
	// returns linear approximation at point ti to point T
	IloExpr intTo(IloExpr & T, IloNum ti);
	// returns the integral of the survival function from the argument to infinity
	IloNum intToInf(IloNum T);
	// returns linear approximation at point ti to point T
	IloExpr intToInf(IloExpr & T, IloNum ti);
	// gives the range to check for T's, argument is integer for number of T's to check 
	// uniformly distributed on the range
	vector<IloNum> getTRange(unsigned int n);
	// gets larger range of T values around the distribution
	vector<IloNum> getLargerTRange(unsigned int n);
	// shifts the distribution
	void shift(IloNum shift_val){a += shift_val; b += shift_val;}
	// prints to file
	void print(string fname, IloNum lower, IloNum upper, IloInt count);
private:
	// Uniform a - lower bound b = upperbound of pdf
	IloNum a, b;
};

class JumpDist
{
public:
	JumpDist(IloNum t);
	~JumpDist(){}
	void shift(IloNum shift_val){t0 += shift_val;}
	IloNum cdf(IloNum t);
	IloNum intTo(IloNum T);
	IloExpr intTo( IloExpr & T);
	IloNum intToInf(IloNum T);
	IloExpr intToInf(IloExpr & T);
	vector<IloNum> getTRange(unsigned int n);
	// worthless
	vector<IloNum> getLargerTRange(unsigned int n){vector<IloNum> temp; return temp;}
private:
	IloNum t0;
};