#include "Soldier.h"
#include "Information.h"
#include "Assets.h"

PlayerSoldier::PlayerSoldier(string name, SyukatsuGame *game, Field *field)
  :Character(name, game, field)
{
  setAttributes(50, 10, 10, 50.0f, Vector3(1.0f, 0.0f, 0.0f)); 
  setModel(Assets::enemies[Information::SOLDIER0]);
}

void PlayerSoldier::update(float deltaTime)
{
  Character::update(deltaTime);
}

