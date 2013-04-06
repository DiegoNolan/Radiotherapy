#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <boost/math/quaternion.hpp>

#define QUAT_NORM_TOL .000001f

using boost::math::quaternion;

void initGL(sf::Window & window);

void initLights();

void handleEvents(sf::Window & window);


class Quat : public quaternion<GLfloat>
{
public:
   Quat(GLfloat a, GLfloat b, GLfloat c, GLfloat d);
   ~Quat(){}

   void normalize();
   Quat conjugate();
private:

};
