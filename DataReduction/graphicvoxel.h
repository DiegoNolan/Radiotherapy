#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <vector>

using std::vector;

class GraphicVoxel
{
public:
                     // x            y              z dimension length
   GraphicVoxel(GLfloat width=0.f, GLfloat height=0.f, GLfloat depth=0.f,
      GLfloat pos_x=0.f, GLfloat pos_y=0.f, GLfloat pos_z=0.f,
      GLfloat red=255., GLfloat green=0., GLfloat blue=0.);
   ~GraphicVoxel(void);
   void display();
   void operator= (const GraphicVoxel & arg);
private:
   GLfloat x, y, z;
   vector<GLfloat> norms;
   vector<GLfloat> verts;
   vector<GLfloat> color;
};

