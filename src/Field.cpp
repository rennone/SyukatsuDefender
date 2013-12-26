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

bool Field::getTouchPoint(const Vector3 &position, const Vector3 &direction, Vector3 &point)
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

