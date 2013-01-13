#include "set.h"

void writeSet(Set & set, string fname)
{
	// write
	std::ofstream output(fname);

	if(!output.is_open()){
		cout << "Error opening " << fname << endl;
	}

	for(unsigned int i=0;i<set.size()-1;++i)
	{
		output << set[i] << "\n";
	}
	output << set[set.size()-1];

	output.close();
}

void readSet(Set & set, string fname)
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
	set.resize(count);

	input.clear();
	input.seekg(0, std::ios_base::beg);

	count = 0;
	while(std::getline(input, linestring))
	{
		std::stringstream ss;
		ss << linestring;
		ss >> set[count];
		++count;
	}
}

Set mergeSets(Set & set1, Set & set2)
{
	Set ret;
	Set::iterator i, j;
	i = set1.begin();
	j = set2.begin();

	while(i != set1.end() && j != set2.end())
	{
		if(*i < *j)
		{
			ret.push_back(*i);
			++i;
		}else{
			ret.push_back(*j);
			++j;
		}
	}

	if(i == set1.end()){
		ret.insert(ret.end(), j, set2.end());
	}else{
		ret.insert(ret.end(), i, set1.end());
	}

	return ret;
}

unsigned choose(unsigned n, unsigned r)
{
   unsigned long long num, den;
   num = den = 1;
   for(unsigned int i=1;i<=r;++i){
      num *= (n-(r-i));
      den *= i;
   }

   return unsigned(num/den);
}