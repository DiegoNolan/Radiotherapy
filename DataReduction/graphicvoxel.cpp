#include "graphicvoxel.h"


GraphicVoxel::GraphicVoxel(GLfloat width, GLfloat height, GLfloat depth,
      GLfloat pos_x, GLfloat pos_y, GLfloat pos_z,
      GLfloat red, GLfloat green, GLfloat blue)
{
   x = pos_x;
   y = pos_y;
   z = pos_z;
   color.resize(3);
   color[0] = red; color[1] = green; color[2] = blue;

   // create vertex array
   verts.resize(72);
   // left
   verts[0] = -width/2.f; verts[1] = -height/2.f; verts[2] = -depth/2.f;
   verts[3] = -width/2.f; verts[4] = -height/2.f; verts[5] =  depth/2.f;
   verts[6] = -width/2.f; verts[7] =  height/2.f; verts[8] =  depth/2.f;
   verts[9] = -width/2.f; verts[10]=  height/2.f; verts[11]= -depth/2.f;
   // right
   verts[12]=  width/2.f; verts[13]= -height/2.f; verts[14]= -depth/2.f;
   verts[15]=  width/2.f; verts[16]= -height/2.f; verts[17]=  depth/2.f;
   verts[18]=  width/2.f; verts[19]=  height/2.f; verts[20]=  depth/2.f;
   verts[21]=  width/2.f; verts[22]=  height/2.f; verts[23]= -depth/2.f;
   // back
   verts[24]= -width/2.f; verts[25]=  height/2.f; verts[26]=  depth/2.f;
   verts[27]= -width/2.f; verts[28]= -height/2.f; verts[29]=  depth/2.f;
   verts[30]=  width/2.f; verts[31]= -height/2.f; verts[32]=  depth/2.f;
   verts[33]=  width/2.f; verts[34]=  height/2.f; verts[35]=  depth/2.f;
   // front
   verts[36]= -width/2.f; verts[37]=  height/2.f; verts[38]= -depth/2.f;
   verts[39]= -width/2.f; verts[40]= -height/2.f; verts[41]= -depth/2.f;
   verts[42]=  width/2.f; verts[43]= -height/2.f; verts[44]= -depth/2.f;
   verts[45]=  width/2.f; verts[46]=  height/2.f; verts[47]= -depth/2.f;
   // top
   verts[48]= -width/2.f; verts[49]=  height/2.f; verts[50]=  depth/2.f;
   verts[51]= -width/2.f; verts[52]=  height/2.f; verts[53]= -depth/2.f;
   verts[54]=  width/2.f; verts[55]=  height/2.f; verts[56]= -depth/2.f;
   verts[57]=  width/2.f; verts[58]=  height/2.f; verts[59]=  depth/2.f;
   // bottom
   verts[60]= -width/2.f; verts[61]= -height/2.f; verts[62]=  depth/2.f;
   verts[63]= -width/2.f; verts[64]= -height/2.f; verts[65]= -depth/2.f;
   verts[66]=  width/2.f; verts[67]= -height/2.f; verts[68]= -depth/2.f;
   verts[69]=  width/2.f; verts[70]= -height/2.f; verts[71]=  depth/2.f;

   norms.resize(72);
   // left
   norms[0] = -1.f; norms[1] =  0.f; norms[2] =  0.f;
   norms[3] = -1.f; norms[4] =  0.f; norms[5] =  0.f;
   norms[6] = -1.f; norms[7] =  0.f; norms[8] =  0.f;
   norms[9] = -1.f; norms[10]=  0.f; norms[11]=  0.f;
   // right
   norms[12]=  1.f; norms[13]=  0.f; norms[14]=  0.f;
   norms[15]=  1.f; norms[16]=  0.f; norms[17]=  0.f;
   norms[18]=  1.f; norms[19]=  0.f; norms[20]=  0.f;
   norms[21]=  1.f; norms[22]=  0.f; norms[23]=  0.f;
   // back
   norms[24]= -0.f; norms[25]=  0.f; norms[26]=  1.f;
   norms[27]= -0.f; norms[28]= -0.f; norms[29]=  1.f;
   norms[30]=  0.f; norms[31]= -0.f; norms[32]=  1.f;
   norms[33]=  0.f; norms[34]=  0.f; norms[35]=  1.f;
   // front
   norms[36]= -0.f; norms[37]=  0.f; norms[38]= -1.f;
   norms[39]= -0.f; norms[40]= -0.f; norms[41]= -1.f;
   norms[42]=  0.f; norms[43]= -0.f; norms[44]= -1.f;
   norms[45]=  0.f; norms[46]=  0.f; norms[47]= -1.f;
   // top
   norms[48]= -0.f; norms[49]=  1.f; norms[50]=  0.f;
   norms[51]= -0.f; norms[52]=  1.f; norms[53]= -0.f;
   norms[54]=  0.f; norms[55]=  1.f; norms[56]= -0.f;
   norms[57]=  0.f; norms[58]=  1.f; norms[59]=  0.f;
   // bottom
   norms[60]= -0.f; norms[61]= -1.f; norms[62]=  0.f;
   norms[63]= -0.f; norms[64]= -1.f; norms[65]= -0.f;
   norms[66]=  0.f; norms[67]= -1.f; norms[68]= -0.f;
   norms[69]=  0.f; norms[70]= -1.f; norms[71]=  0.f;
}


GraphicVoxel::~GraphicVoxel(void)
{
}

void GraphicVoxel::display()
{
   glColor3fv(&color[0]);
   glEnableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_VERTEX_ARRAY);  
   glNormalPointer(GL_FLOAT, 0, &norms[0]);
   glVertexPointer(3, GL_FLOAT, 0, &verts[0]);

   glPushMatrix();

   glTranslatef(x, y, z);

   glDrawArrays(GL_QUADS, 0, verts.size()/3);

   glPopMatrix();

   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_VERTEX_ARRAY);
}

void GraphicVoxel::operator= (const GraphicVoxel & arg)
{
   x = arg.x;
   y = arg.y;
   z = arg.z;
   norms = arg.norms;
   verts = arg.verts;
   color = arg.color;
}