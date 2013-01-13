#pragma once

#include <vector>

using std::vector;

// struct for beamlmjallnerset vector
typedef struct
{
	short rx;
	short ry;
} vec2;

class BeamLetMapping
{
public:
   BeamLetMapping(){}
   BeamLetMapping(vector<vec2> beamletCounts);
   unsigned getBeamlet(short rx, short ry, short beam_ind);
   ~BeamLetMapping(void);
private:
   // vector of beamlets
   vector<vec2> beamlets;
};

