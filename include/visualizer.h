#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <boost/math/quaternion.hpp>

#define QUAT_NORM_TOL .000001f

using boost::math::quaternion;

void initGL(sf::Window & window);

void initLights();

void handleEvents(sf::Window & window);


// a is rotation, b is x, c is y, d is z
class GLQuaternion : public quaternion<GLfloat>
{
public:
   GLQuaternion(GLfloat a, GLfloat b, GLfloat c, GLfloat d) :
      quaternion<GLfloat>(a, b, c, d)
   {}
   ~GLQuaternion(){}

   void normalise();
   GLQuaternion conjugate();

   sf::Vector3<GLfloat> operator* (sf::Vector3<GLfloat> vec);

private:

};
