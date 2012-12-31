
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <set>

#include "graph.h"
#include "parsedata.h"
#include "set.h"
#include "graphicvoxel.h"
#include "visualizer.h"
#include "graphicorgan.h"

using std::vector;
using std::string;
using std::cout;
using std::endl;



int main()
{
   vector<string> fnames;
   // enter file names
	fnames.push_back("Spinalcord+0.5");
	fnames.push_back("L PAROTID");
	fnames.push_back("pituitary");
	fnames.push_back("PTV 1");
	fnames.push_back("R PAROTID");
	
	vector<std::set<unsigned> > organSets(fnames.size());
   Sparse_Matrix<IloInt> vox;
   VoxelMapping voxmap;

   // parse data and write data to files
   parseOrganData(vox, organSets, voxmap, fnames);

	cout << "done " << endl;

   sf::Window window(sf::VideoMode(900, 600), "test");

   initGL(window);

   vector<GraphicOrgan> graphicOrgans(organSets.size());

   for(unsigned i=0;i<graphicOrgans.size();++i)
   {
      graphicOrgans[i] = GraphicOrgan(organSets[i],voxmap,
         organ_colors[i][0], organ_colors[i][1], organ_colors[i][2]);
   }

   while(1)
   {
      handleEvents(window);

      glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

      for(unsigned i=0;i<graphicOrgans.size();++i)
      {
         graphicOrgans[i].display();
      }

      window.display();
   }
  
   std::cin.ignore();
   return 0;
}

