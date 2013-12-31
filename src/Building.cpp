#include "Building.h"
#include "GL/glut.h"

Building::Building(std::string _name, SyukatsuGame *_game, Field *_field)
  :Actor(_name, _game)
  ,field(_field)
  ,position(Vector3(0, 0, 0))
{
  picked = false;
  setAttributes(100);
}

void Building::render(float deltaTime)
{
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
  
  glPushMatrix();

  if(picked) {
    glColor3d(1.0, 0.0, 0.0);
  }
  else {
    glColor3d(0.0, 1.0, 0.0);
  }

  glTranslatef(position.x, position.y, position.z);
  glutSolidCube(40);
  glPopMatrix();

  glPopAttrib();

  Actor::render(deltaTime);
}

void Building::update(float deltaTime)
{
  if(hp <= 0) {
    setStatus(Actor::Dead);
  }

  Actor::update(deltaTime);
}

bool Building::gotDamage(int value)
{
  hp -= value;

  if(hp <= 0) return true;

  return false;
}
