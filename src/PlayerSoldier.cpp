#include "PlayerSoldier.h"
#include "Debugger.h"
#include "PlayScene.h"

PlayerSoldier::PlayerSoldier(string name, SyukatsuGame *game, Field *field)
  :Character(name, game, field)
{
  this->setAttributes(50, 10, 10, 50.0f, Vector3(1.0f, 0.0f, 0.0f)); 
}

void PlayerSoldier::update(float deltaTime)
{
  Character::update(deltaTime);
}

