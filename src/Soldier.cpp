#include "Soldier.h"
#include "Information.h"
#include "Assets.h"

Soldier::Soldier(string name, SyukatsuGame *game, Field *field, int _level)
  :Character(name, game, field, _level)
{
  setAttributes(Information::Enemies::SOLDIER); 
  setModel(Assets::enemies[Information::SOLDIER]);
}

void Soldier::update(float deltaTime)
{
  Character::update(deltaTime);
}

