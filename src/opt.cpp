#include "opt.h"

Opt::Opt(IloEnv & env) : ArgMin(env), Duals(env)
{

}


void Opt::write(string fname)
{
	std::ofstream output(fname.c_str());

	if(!output.is_open()){
		cout << "Error opening " << fname << endl;
	}

	for(unsigned int i=0;i<ArgMin.getSize()-1;++i)
	{
		output << ArgMin[i] << "\n";
	}

	output << ArgMin[ArgMin.getSize()-1];

	output.close();
}