#include "graphicbeamlet.h"

double to_rad(double angle)
{
   return (PI/180.)*angle;
}

GraphicBeamlet::GraphicBeamlet(GLfloat radius, GLfloat length,
      GLfloat x_pos, GLfloat y_pos, GLfloat z_pos,
      GLfloat p_v, GLfloat q_v, GLfloat r_v, GLfloat angle,
      unsigned sides)
{
   p = p_v;
   q = q_v;
   r = r_v;
   theta = angle;
   x = x_pos;
   y = y_pos;
   z = z_pos;

   // create vert data and normal data
   verts.resize(12*sides);
   norms.resize(12*sides);
   for(unsigned i=0;i<sides;++i)
   {
      // vertices
      verts[i*12]    = radius*GLfloat(cos(to_rad((360.*double(i))/double(sides))));
      verts[i*12+1]  = radius*GLfloat(sin(to_rad((360.*double(i))/double(sides))));
      verts[i*12+2]  = length/2.f;
      verts[i*12+3]  = radius*GLfloat((cos(to_rad(360.*double(i))/double(sides))));
      verts[i*12+4]  = radius*GLfloat((sin(to_rad(360.*double(i))/double(sides))));
      verts[i*12+5]  = -length/2.f;
      verts[i*12+6]  = radius*GLfloat((cos(to_rad(360.*double(i+1))/double(sides))));
      verts[i*12+7]  = radius*GLfloat((sin(to_rad(360.*double(i+1))/double(sides))));
      verts[i*12+8]  = -length/2.f;
      verts[i*12+9]  = radius*GLfloat((cos(to_rad(360.*double(i+1))/double(sides))));
      verts[i*12+10] = radius*GLfloat((sin(to_rad(360.*double(i+1))/double(sides))));
      verts[i*12+11] = length/2.f;

      // normals
      norms[i*12]    = GLfloat(cos(to_rad(180./double(sides)+360.*double(i)/double(sides))));
      norms[i*12+1]  = GLfloat(sin(to_rad(180./double(sides)+360.*double(i)/double(sides))));
      norms[i*12+2]  = 0.f;
      norms[i*12+3] = norms[i*12+6] = norms[i*12+9] = norms[i*12];
      norms[i*12+4] = norms[i*12+7] = norms[i*12+10] = norms[i*12+1];
      norms[i*12+5] = norms[i*12+8] = norms[i*12+11] = norms[i*12+2];
   }

   color.resize(4);
   color[0] = 0.f;
   color[1] = 0.f;
   color[2] = 1.f;
   color[3] = .2f;
}

void GraphicBeamlet::display()
{
   glColor4fv(&color[0]);
   glEnableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_VERTEX_ARRAY);
   glNormalPointer(GL_FLOAT,0, &norms[0]);
   glVertexPointer(3, GL_FLOAT, 0, &verts[0]);

   glPushMatrix();

   glRotatef(theta, p, q, r);
   glTranslatef(x, y, z);

   glDrawArrays(GL_QUADS, 0, GLsizei(verts.size()/3));

   glPopMatrix();

   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_VERTEX_ARRAY);
}

void GraphicBeamlet::operator= (const GraphicBeamlet & arg)
{
   x = arg.x;
   y = arg.y;
   z = arg.z;
   p = arg.p;
   q = arg.p;
   theta = arg.theta;
   color = arg.color;
   verts = arg.verts;
   norms = arg.norms;
}
