#include "Assets.h"
#include "Information.h"
#include "Knight.h"

Knight::Knight(string name, SyukatsuGame *game, Field *field, int _level)
  :Character(name, game, field, _level)
{
  this->setAttributes(Information::Enemies::KNIGHT); 
  setModel(Assets::enemies[Information::Enemies::KNIGHT]);
}

void Knight::update(float deltaTime)
{
  Character::update(deltaTime);
}
