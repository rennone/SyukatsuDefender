#include "Knight.h"

Knight::Knight(string name, SyukatsuGame *game, Field *field)
  :Character(name, game, field)
{
  this->setAttributes(40, 10, 20, 70.0f, Vector3(0.0f, 1.0f, 0.0f)); 
}

void Knight::update(float deltaTime)
{
  Character::update(deltaTime);
}
