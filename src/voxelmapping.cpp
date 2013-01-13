#include "voxelmapping.h"


VoxelMapping::VoxelMapping(float x_spacing, float y_spacing, float z_spacing,
      float x_start, float y_start, float z_start,
      unsigned x_incr, unsigned y_incr, unsigned z_incr)
{
   xs = x_spacing;
   ys = y_spacing;
   zs = z_spacing;

   xi = x_start;
   yi = y_start;
   zi = z_start;

   xc = x_incr;
   yc = y_incr;
   zc = z_incr;
}


VoxelMapping::~VoxelMapping(void)
{
}

float VoxelMapping::getX(unsigned index)
{
   return float(index%yc)*xs+xi;
}

float VoxelMapping::getY(unsigned index)
{
   return float((index/yc)%((zc)/(yc)))*ys+yi;
}

float VoxelMapping::getZ(unsigned index)
{
   return float(index/zc)*zs+zi;
}

void VoxelMapping::print()
{
   cout << "Initial values" << endl;
   cout << xi << " " << yi << " " << zi << endl;
   cout << "Spacing" << endl;
   cout << xs << " " << ys << " " << zs << endl;
   cout << "increments" << endl;
   cout << xc << " " << yc << " " << zc << endl;
}

void VoxelMapping::operator= (VoxelMapping arg)
{
   xs = arg.xs;
   ys = arg.ys;
   zs = arg.zs;
   xi = arg.xi;
   yi = arg.yi;
   zi = arg.zi;
   xc = arg.xc;
   yc = arg.yc;
   zc = arg.zc;
}

void VoxelMapping::write(string fname)
{
   std::ofstream out;
   out.open(fname.c_str());

   if( out )
   {
      out << xs << " " << ys << " " << zs << endl;
      out << xi << " " << yi << " " << zi << endl;
      out << xc << " " << yc << " " << zc;
   }
   else
   {
      cout << "Error opening " << fname << endl;
   }

   out.close();
}

void VoxelMapping::read(string fname)
{
   std::ifstream in;
   in.open(fname.c_str());

   if( in )
   {
      in >> xs >> ys >> zs;
      in >> xi >> yi >> zi;
      in >> xc >> yc >> zc;
   }
   else
   {
      cout << "Error opening " << fname << endl;
   }

   in.close();
}
