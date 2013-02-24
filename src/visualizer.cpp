#include "visualizer.h"

void initGL(sf::Window & window)
{
   gluPerspective(40., float(window.getSize().x)/
      float(window.getSize().y), 1., 10000.);

   glEnable(GL_DEPTH_TEST);

   glEnable(GL_LIGHTING);

   glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glEnable( GL_COLOR_MATERIAL );

   GLfloat lightKa[] = {.1f, .1f, .1f, 1.0f};
   GLfloat lightKd[] = {1.f, 1.f, 1.f, 1.0f}; 
   GLfloat lightKs[] = {0.5, 0.5, 0.5, 0.5};
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

   float lightPos[4] = {1000.f, 1000.f, 1000.f, 0};
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

   glEnable(GL_LIGHT0);
}

void initLights()
{
   GLfloat lightKa[] = {.1f, .1f, .1f, 1.0f};
   GLfloat lightKd[] = {1.f, 1.f, 1.f, 1.0f}; 
   GLfloat lightKs[] = {0.5, 0.5, 0.5, 0.5};
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

   float lightPos[4] = {100, 1, 0, 0};
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

   glEnable(GL_LIGHT0);
}

void handleEvents(sf::Window & window)
{
   quaternion<GLfloat> test(1.f, 1.f, 1.f, 1.f);

   sf::Event event;
   while(window.pollEvent(event))
   {
      if(event.type == sf::Event::Closed){
         window.close();
      }

      if(event.type == sf::Event::MouseWheelMoved){
         glTranslatef(0.f, 0.f, 5.f*float(event.mouseWheel.delta));
      }

      if(event.type == sf::Event::KeyPressed){
         switch(event.key.code)
         {
         case sf::Keyboard::Left:
            glRotatef(1.f, 0.f, 1.f, 0.f);
            break;
         case sf::Keyboard::Right:
            glRotatef(-1.f, 0.f, 1.f, 0.f);
            break;
         case sf::Keyboard::Up:
            glRotatef(1.f, 1.f, 0.f, 0.f);
            break;
         case sf::Keyboard::Down:
            glRotatef(-1.f, 1.f, 0.f, 0.f);
            break;
         default:
            break;
         }
      }
   }
}

void GLQuaternion::normalise()
{
   GLfloat mag2 = a*a + b*b + c*c + d*d;
   if(abs(float(mag2)) > QUAT_NORM_TOL && abs(float(mag2)-1.f) > QUAT_NORM_TOL)
   {
      float mag = sqrt(float(mag2));
      a /= mag;
      b /= mag;
      c /= mag;
      d /= mag;
   }
}

GLQuaternion GLQuaternion::conjugate()
{
   return GLQuaternion(a, -b, -c, -d);
}

sf::Vector3<GLfloat> GLQuaternion::operator* (sf::Vector3<GLfloat> vec)
{
   GLfloat mag = sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
   vec.x /= mag;
   vec.y /= mag;
   vec.z /= mag;

   GLQuaternion vecQuat(0.f, vec.x, vec.y, vec.z);

  // GLQuaternion resQuat = vecQuat*conjugate();

   return sf::Vector3<GLfloat>();
}