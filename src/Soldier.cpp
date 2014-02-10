#include "Soldier.h"
#include "Information.h"
#include "Assets.h"

Soldier::Soldier(string name, SyukatsuGame *game, Field *field)
  :Character(name, game, field)
{
  setAttributes(Information::Enemies::SOLDIER); 
  setModel(Assets::enemies[Information::SOLDIER]);
}

void Soldier::update(float deltaTime)
{
  Character::update(deltaTime);
}

