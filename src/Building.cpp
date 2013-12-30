#include "Building.h"
#include "GL/glut.h"

Building::Building(std::string _name, SyukatsuGame *_game, Field *_field)
  :Actor(_name, _game)
  ,field(_field)
  ,position(Vector3(0, 0, 0))
{
}

void Building::render(float deltaTime)
{
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
  
  glPushMatrix();
  glTranslatef(position.x, position.y, position.z);
  glutSolidCube(40);
  glPopMatrix();

  glPopAttrib();

  Actor::render(deltaTime);
}
