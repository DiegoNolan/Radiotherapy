#include "disthelper.h"


DistHelper::DistHelper(Sparse_Matrix<IloInt> *v, Set *x, Set* y, IloNumArray *what)
{
   vox = v;
   X = x;
   Y = y;
   w = what;
}

IloNum DistHelper::distOnX( IloNum t)
{
  unsigned int sum=0;
  for(unsigned int i=0;i<X->size();++i)
  {
     if(D((*X)[i], *vox, *w) <= t){
        ++sum;
     }
  }

  return IloNum(sum)/IloNum(X->size());
}

IloNum DistHelper::distOnY(IloNum t)
{
  unsigned int sum=0;
  for(unsigned int i=0;i<Y->size();++i)
  {
     if(D((*Y)[i], *vox, *w) <= t){
        ++sum;
     }
  }

  return IloNum(sum)/IloNum(Y->size());
}

IloNum DistHelper::intDistOnX(IloNum T)
{
  IloNum sum=0.;
  for(unsigned int i=0;i<X->size();++i)
  {
     if( D((*X)[i], *vox, *w) - T > 0){
        sum += D((*X)[i], *vox, *w) - T;
     }
  }


  return sum/IloNum(X->size());
}

IloNum DistHelper::intDistOnY(IloNum T)
{
  IloNum sum=0.;
  for(unsigned int i=0;i<Y->size();++i)
  {
     if( T - D((*Y)[i], *vox, *w)> 0){
        sum += T - D((*Y)[i], *vox, *w);
     }
  }


  return sum/IloNum(Y->size());
}

void DistHelper::print(string fname, IloNum lower, IloNum upper, IloNum count)
{
  std::ofstream out;

  out.open(fname.c_str());
  out.width(20);

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
}
