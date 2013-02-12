#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <cmath>

#define PI 3.14159

double to_rad(double angle);

using std::vector;

class GraphicBeamlet
{
public:
   GraphicBeamlet(){}
   GraphicBeamlet(GLfloat radius, GLfloat length,
      GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
      GLfloat p_v, GLfloat q_v, GLfloat r_v, GLfloat angle,
      unsigned sides = 10);
   ~GraphicBeamlet(){}
   void display();
   void operator=(const GraphicBeamlet & arg);
private:
   GLfloat x, y, z;
   // normal vector and the angle to be rotated around
   // not typical euler angles
   GLfloat p, q, r, theta;
   vector<GLfloat> color;
   vector<GLfloat> verts;
   vector<GLfloat> norms;
};

