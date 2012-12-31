#pragma once

// helper class for mapping the index of a voxel to its location
// in x,y,z coordinates

#include <iostream>

using std::cout;
using std::endl;

class VoxelMapping
{
public:
   VoxelMapping(float x_spacing=0.f, float y_spacing=0.f, float z_spacing=0.f,
      float x_start=0.f, float y_start=0.f, float z_start=0.f,
      unsigned x_incr=0, unsigned y_incr=0, unsigned z_incr=0);
   ~VoxelMapping(void);
   float getX(unsigned index);
   float getY(unsigned index);
   float getZ(unsigned index);
   void print();
   void operator= (VoxelMapping arg);
   float width(){return xs;}
   float height(){return ys;}
   float depth(){return zs;}
private:
   float xs, ys, zs, xi, yi, zi;
   unsigned xc, yc, zc;
};

