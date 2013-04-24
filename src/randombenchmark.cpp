#include "randombenchmark.h"

void randomSetAndPTV(Set & ptv, Set & oar, unsigned rows, unsigned cols,
   unsigned lowerPTV, unsigned upperPTV, unsigned lowerOAR, unsigned upperOAR)
{
   set<IloInt> organ, target;
   unsigned total = rows*cols;
   srand(time(NULL));

   unsigned ptvSize = rand()%(upperPTV-lowerPTV)+lowerPTV;
   unsigned oarSize = rand()%(upperOAR-lowerOAR)+lowerOAR;

   unsigned temp;
   while(target.size() <= ptvSize)
   {
      temp = rand()%total;
      if(target.find(temp) == target.end() &&
         organ.find(temp) == organ.end()) 
      {
         target.insert(temp);
      }
   }

   while(organ.size() <= oarSize)
   {
      temp = rand()%total;
      if(target.find(temp) == target.end() &&
         organ.find(temp) == organ.end()) 
      {
         organ.insert(temp);
      }
   }

   ptv.clear(); oar.clear();
   ptv.insert(ptv.begin(), target.begin(), target.end());
   oar.insert(oar.begin(), organ.begin(), organ.end());
}
