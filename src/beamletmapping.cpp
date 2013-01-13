#include "beamLetmapping.h"


BeamLetMapping::BeamLetMapping(vector<vec2> beamletCounts)
{
   beamlets = beamletCounts;
}


BeamLetMapping::~BeamLetMapping(void)
{
}

unsigned BeamLetMapping::getBeamlet(short rx, short ry, short beam_ind)
{
   unsigned beamlet = 0;
   for(short k=0;k<beam_ind;++k)
   {
      beamlet += beamlets[k].rx*beamlets[k].ry;
   }

   beamlet += beamlets[beam_ind].rx*ry + rx;

   return beamlet;
}


