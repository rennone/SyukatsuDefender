#include "Field.h"
#include "Assets.h"
#include "SimpleObjectFactory.h"

#include <syukatsu/GL/glut.h>
Field::Field(string name, SyukatsuGame *game)
  :Actor(name, game)
  ,position(Vector3(0,0,0))
  ,size(Vector3(1000, 0, 1000))
{
  
}

Field::~Field()
{
  
}

void Field::render(float deltaTime)
{
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

  Assets::textureAtlas->bind();
  
  //vertex normal texture
  float vertices[] =
    {
      -size.x/2, -size.y/2, -size.z/2,  0,1,
      +size.x/2, -size.y/2, -size.z/2,  1,1,
      +size.x/2, -size.y/2, +size.z/2,  1,0,
      -size.x/2, -size.y/2, +size.z/2,  0,0,
    };

  GLuint indices[] = { 0,  1,  2,  3,  0,  2, };

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  
  glVertexPointer(3, GL_FLOAT, 5*sizeof(float), vertices  );
  glTexCoordPointer(2, GL_FLOAT, 5*sizeof(float), vertices+3);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);  
  glBindTexture(GL_TEXTURE_2D, 0);

  drawAxis();  //SimplePbjectFactory  
  
  glPopAttrib();
}

bool Field::getCollisionPoint(const Vector3 &position, const Vector3 &direction, Vector3 &point)
{
  if(direction.y == 0)
    return false;

  float t = -position.y/direction.y;
  float x = position.x + direction.x*t;
  float z = position.z + direction.z*t;

  if(x < this->position.x-this->size.x/2 ||
     this->position.x+this->size.x/2 < x ||
     z < this->position.z-this->size.z/2 ||
     this->position.z+this->size.z/2 < z)
    return false;

  point.x = x;
  point.y = 0;
  point.z = z;
  return true;  
}

Vector3 Field::collision(const Vector3 &pos, const Vector3 &move)
{
  const Vector3 after = pos + move;
  
  float left = -size.x/2;  
  if( left > after.x)
  {
    float t = (left-pos.x)/move.x;
    Vector3 point = pos + t*move;    
    point.x += t*move.x;  // equal to point = point + normal.dot(move*t)*normal 
    return point;    
  }

  float right = +size.x/2;
  if( after.x > right)
  {
    float t = (right-pos.x)/move.x;
    Vector3 point = pos + t*move;    
    point.x -= t*move.x;  // equal to point = point + normal.dot(move*t)*normal 
    return point;    
  }

  float near =  - size.z/2;
  if( after.z < near)
  {
    float t = (near-pos.z)/move.z;
    Vector3 point = pos + t*move;    
    point.z += t*move.z;  // equal to point = point + normal.dot(move*t)*normal 
    return point;    
  }  
  
  float far = + size.z/2;
  if( after.z > far)
  {
    float t = (far-pos.z)/move.z;
    Vector3 point = pos + t*move;    
    point.z -= t*move.z;  // equal to point = point + normal.dot(move*t)*normal 
    return point;    
  }

  return after;  
}


