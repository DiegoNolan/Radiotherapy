#include "disthelper.h"


DistHelper::DistHelper(Sparse_Matrix<IloInt> *v, Set * region, IloNumArray *what)
{
   vox = v;
   Region = region;
   w = what;
}

void DistHelper::operator= (DistHelper & arg)
{
   vox = arg.vox;
   Region = arg.Region;
   w = arg.w;
}

IloNum DistHelper::dist( IloNum t)
{
  unsigned int sum=0;
  for(unsigned int i=0;i<Region->size();++i)
  {
     if(D((*Region)[i], *vox, *w) <= t){
        ++sum;
     }
  }

  return IloNum(sum)/IloNum(Region->size());
}

IloNum DistHelper::intToInf(IloNum T)
{
  IloNum sum=0.;
  for(unsigned int i=0;i<Region->size();++i)
  {
     if( D((*Region)[i], *vox, *w) - T > 0){
        sum += D((*Region)[i], *vox, *w) - T;
     }
  }


  return sum/IloNum(Region->size());
}

IloNum DistHelper::intTo(IloNum T)
{
  IloNum sum=0.;
  for(unsigned int i=0;i<Region->size();++i)
  {
     if( T - D((*Region)[i], *vox, *w)> 0){
        sum += T - D((*Region)[i], *vox, *w);
     }
  }


  return sum/IloNum(Region->size());
}
/*
void DistHelper::print(string fname, IloNum lower, IloNum upper, IloNum count)
{
  std::ofstream out;

  out.open(fname.c_str());
  out.width(20);k

  if(!out.is_open()){
     cout << "Error: could not open " << fname << '\n';
  }

  IloNum spacing = (upper - lower)/(IloNum(count)-1);

  out << "Dist on Y\n";
  for(IloNum t = lower;t<=upper;t+=spacing)
  {
     out.width(20);
     out << t << "  " << distOnY(t) << "  " << intDistOnY(t) << '\n';
  }

  out << "Dist on X\n";
  for(IloNum t = lower;t<=upper;t+=spacing)
  {
     out.width(20);
     out << t << "  " << distOnX(t) << "  " << intDistOnX(t) << '\n';
  }

  out.close();
} */
