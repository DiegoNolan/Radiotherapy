#include "set.h"

void writeSet(std::set<unsigned> & set, string fname)
{
	// write
	std::ofstream output(fname);

	if(!output.is_open()){
		cout << "Error opening " << fname << endl;
	}

   for(std::set<unsigned>::iterator it=set.begin();
      it != (--set.end());++it)
   {
      output << *it << "\n";
   }

	output << *(--set.end());

	output.close();
}

void readSet(std::set<unsigned> & set, string fname)
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
	vector<unsigned> temp(count);

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

   set.clear();
   set.insert(temp.begin(), temp.end());
}

unsigned int factorial(unsigned int n)
{
	if(n == 0){
		return 1;
	}

	return factorial(n-1);
}

unsigned int choose(unsigned int n, unsigned int r)
{
	return (factorial(n))/(factorial(r)*factorial(n-r));
}
