#include "SimpleObjectFactory.h"
#include <syukatsu/GL/freeglut.h>

void drawGrid(const float xNum,const float zNum,const float size)
{
  glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT| GL_ENABLE_BIT);
  
  float left  = -xNum/2.0*size;
  float right = +xNum/2.0*size;
  float front = -zNum/2.0*size;
  float back  = +zNum/2.0*size;
  
  glColor3d(0.2, 0.3, 0.5);
  
  glBegin(GL_POLYGON);
  glVertex3d( left , -1, back  );
  glVertex3d( left , -1, front );
  glVertex3d( right, -1, front );
  glVertex3d( right, -1, back  );
  glEnd();
        
  glColor3d(1.0, 0.0, 0.0);
  for(int i=0; i<=xNum; i++){
    float x = (i-xNum/2.0)*size;
    glBegin(GL_LINES);
    glVertex3d(x, 0, front);
    glVertex3d(x, 0, back);
    glEnd();
  }
        
  for(int i=0; i<=zNum; i++){
    float z = (i-zNum/2.0)*size;
    glBegin(GL_LINES);
    glVertex3d(left , 0, z);
    glVertex3d(right, 0, z);
    glEnd();
  }
  
  glPopAttrib();
}

void drawTexCube(const float size,const int tex,const bool reverse)
{  
  glPushAttrib( GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);  
  glBindTexture(GL_TEXTURE_2D, tex);  
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  const float n = reverse ? -1 : 1;
  
  //vertex normal texture
  float vertices[] =
    {
      //bottom
      -size, -size, -size,  0,-n,0,  0,1,
      +size, -size, -size,  0,-n,0,  1,1,
      +size, -size, +size,  0,-n,0,  1,0,
      -size, -size, +size,  0,-n,0,  0,0,

      //top
      -size, +size, -size,  0,+n,0,  0,0,
      +size, +size, -size,  0,+n,0,  1,0,
      +size, +size, +size,  0,+n,0,  1,1,
      -size, +size, +size,  0,+n,0,  0,1,

      //left
      -size, -size, -size,  -n,0,0,  0,1,
      -size, +size, -size,  -n,0,0,  0,0,
      -size, +size, +size,  -n,0,0,  1,0,
      -size, -size, +size,  -n,0,0,  1,1,

      //right
      +size, -size, -size,  +n,0,0,  1,1,
      +size, +size, -size,  +n,0,0,  1,0,
      +size, +size, +size,  +n,0,0,  0,0,
      +size, -size, +size,  +n,0,0,  0,1,

      //front
      -size, -size, +size,  0,0,+n,  0,1,
      -size, +size, +size,  0,0,+n,  0,0,
      +size, +size, +size,  0,0,+n,  1,0,
      +size, -size, +size,  0,0,+n,  1,1,

      //back
      -size, -size, -size,  0,0,-n,  1,1,
      -size, +size, -size,  0,0,-n,  1,0,
      +size, +size, -size,  0,0,-n,  0,0,
      +size, -size, -size,  0,0,-n,  0,1      
    };

  GLuint indices[] =
    {
       0,  1,  2,  3,  0,  2,
       4,  5,  6,  7,  4,  6,
       8,  9, 10, 11,  8, 10,
       12, 13, 14, 15, 12, 14,
       16, 17, 18, 19, 16, 18,
       20, 21, 22, 23, 20, 22
    };

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  
  glVertexPointer(3,  GL_FLOAT, 8*sizeof(float), vertices  );
  glNormalPointer(    GL_FLOAT, 8*sizeof(float), vertices+3);
  glTexCoordPointer(2,GL_FLOAT, 8*sizeof(float), vertices+6);  
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  
  glBindTexture(GL_TEXTURE_2D, 0);
  
  glPopAttrib();
}

void drawTexture(const Vector3 &position, const Vector3 &normal, const float size, const TextureRegion *region)
{  
  glPushAttrib(GL_CURRENT_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);
  region->texture->bind();
  
  Vector3 tmp;
  if( normal.x == 0 && normal.z == 0)
    tmp.set(1,0,0);
  else
    tmp.set(0,1,0);  
  
  Vector3 axis1 = normal.cross(tmp);
  axis1.normalize();
  
  Vector3 axis2 = normal.cross(axis1);
  axis2.normalize();

  axis1 *= size/2;
  axis2 *= size/2;
  Vector3 leftBottom  = position - axis1 - axis2;
  Vector3 rightBottom = position + axis1 - axis2;
  Vector3 rightTop    = position + axis1 + axis2;
  Vector3 leftTop     = position - axis1 + axis2;
   //vertex normal texture
  float vertices[] =
    {
      leftBottom.x, leftBottom.y, leftBottom.z,  normal.x, normal.y, normal.z,  region->u1, region->v1,
      rightBottom.x, rightBottom.y, rightBottom.z,  normal.x, normal.y, normal.z,  region->u2, region->v1,
      rightTop.x, rightTop.y, rightTop.z,  normal.x, normal.y, normal.z,  region->u2, region->v2,
      
      rightTop.x, rightTop.y, rightTop.z,  normal.x, normal.y, normal.z,  region->u2, region->v2,
      leftTop.x, leftTop.y, leftTop.z,  normal.x, normal.y, normal.z,  region->u1, region->v2,
      leftBottom.x, leftBottom.y, leftBottom.z,  normal.x, normal.y, normal.z,  region->u1, region->v1,
    };

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  
  glVertexPointer(3,  GL_FLOAT, 8*sizeof(float), vertices  );
  glNormalPointer(    GL_FLOAT, 8*sizeof(float), vertices+3);
  glTexCoordPointer(2,GL_FLOAT, 8*sizeof(float), vertices+6);  
  glDrawArrays(GL_TRIANGLES, 0, 6);
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  glPopAttrib();
}

void drawTexture(const Vector3 &position, const Vector3 &normal, const Vector2 &size, const TextureRegion *region)
{  
  glPushAttrib(GL_CURRENT_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);
  region->texture->bind();
  
  Vector3 tmp;
  if( normal.x == 0 && normal.z == 0)
    tmp.set(1,0,0);
  else
    tmp.set(0,1,0);  
  
  Vector3 axis1 = normal.cross(tmp);
  axis1.normalize();
  
  Vector3 axis2 = normal.cross(axis1);
  axis2.normalize();

  axis1 *= size.x/2;
  axis2 *= size.y/2;
  Vector3 leftBottom  = position - axis1 - axis2;
  Vector3 rightBottom = position + axis1 - axis2;
  Vector3 rightTop    = position + axis1 + axis2;
  Vector3 leftTop     = position - axis1 + axis2;
   //vertex normal texture
  float vertices[] =
    {
      leftBottom.x, leftBottom.y, leftBottom.z,  normal.x, normal.y, normal.z,  region->u1, region->v1,
      rightBottom.x, rightBottom.y, rightBottom.z,  normal.x, normal.y, normal.z,  region->u2, region->v1,
      rightTop.x, rightTop.y, rightTop.z,  normal.x, normal.y, normal.z,  region->u2, region->v2,
      
      rightTop.x, rightTop.y, rightTop.z,  normal.x, normal.y, normal.z,  region->u2, region->v2,
      leftTop.x, leftTop.y, leftTop.z,  normal.x, normal.y, normal.z,  region->u1, region->v2,
      leftBottom.x, leftBottom.y, leftBottom.z,  normal.x, normal.y, normal.z,  region->u1, region->v1,
    };

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  
  glVertexPointer(3,  GL_FLOAT, 8*sizeof(float), vertices  );
  glNormalPointer(    GL_FLOAT, 8*sizeof(float), vertices+3);
  glTexCoordPointer(2,GL_FLOAT, 8*sizeof(float), vertices+6);  
  glDrawArrays(GL_TRIANGLES, 0, 6);
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  glPopAttrib();
}

void drawAxis()
{
  glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
  glDisable(GL_LIGHTING);
  glColor3d(0,0,1);
  glBegin(GL_LINES);
  glVertex3d(0,0,0);
  glVertex3d(0,0,1000);  
  glEnd();

  glColor3d(0,1,0);
  glBegin(GL_LINES);
  glVertex3d(0,0,0);
  glVertex3d(0,1000,0);  
  glEnd();

  glColor3d(1,0,0);
  glBegin(GL_LINES);
  glVertex3d(0,0,0);
  glVertex3d(1000,0,0);  
  glEnd();

  glPopAttrib();
}
