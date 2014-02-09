#include "Soldier.h"
#include "Information.h"
#include "Assets.h"

PlayerSoldier::PlayerSoldier(string name, SyukatsuGame *game, Field *field)
  :Character(name, game, field)
{
  setAttributes(Information::Enemies::SOLDIER); 
  setModel(Assets::enemies[Information::SOLDIER]);
}

void PlayerSoldier::update(float deltaTime)
{
  Character::update(deltaTime);
}

