#pragma once

#include <vector>
#include <set>

#include "graphicvoxel.h"
#include "voxelmapping.h"
#include "set.h"

using std::vector;

const float organ_colors[][3]={
   {255.f, 255.f, 0.f},
   {255.f, 0.f, 255.f},
   {0.f, 255.f, 255.f},
   {0.f, 2.55f, 0.f},
   {0.f, 0.f, 255.f},
   {255.f, 0.f, 0.f}}; 


class GraphicOrgan
{
public:
   GraphicOrgan(){}
   GraphicOrgan(std::set<unsigned> & organset, VoxelMapping & voxmap,
      float red, float green, float blue);
   ~GraphicOrgan(void);
   void display();
   void operator= (GraphicOrgan & arg){voxels = arg.voxels;}
private:
   vector<GraphicVoxel> voxels;
};

