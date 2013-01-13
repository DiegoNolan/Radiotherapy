#include "graphicorgan.h"


GraphicOrgan::GraphicOrgan(std::set<unsigned> & organset, VoxelMapping & voxmap,
   float red, float green, float blue)
{
   voxels.resize(organset.size());
   float width, height, depth;
   width = voxmap.width();
   height = voxmap.height();
   depth = voxmap.depth();

   cout << "dimensions" << endl;
   cout << width << " " << height << " " << depth << endl;

   unsigned i=0;
   for(std::set<unsigned>::iterator it=organset.begin();
      it != organset.end();++it)
   {
      voxels[i] = GraphicVoxel(width, height, depth, voxmap.getX(*it),
         voxmap.getY(*it), voxmap.getZ(*it),
         red, green, blue);
      ++i;
   }
}


GraphicOrgan::~GraphicOrgan(void)
{
}

void GraphicOrgan::display()
{
   for(unsigned i=0;i<voxels.size();++i)
   {
      voxels[i].display();
   }
}
