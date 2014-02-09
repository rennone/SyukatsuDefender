#include "Assets.h"
#include "Information.h"
#include "Knight.h"

Knight::Knight(string name, SyukatsuGame *game, Field *field)
  :Character(name, game, field)
{
  this->setAttributes(Information::Enemies::KNIGHT); 
  setModel(Assets::enemies[Information::Enemies::KNIGHT]);
}

void Knight::update(float deltaTime)
{
  Character::update(deltaTime);
}
