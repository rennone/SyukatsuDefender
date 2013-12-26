#include "EnemySoldier.h"

EnemySoldier::EnemySoldier(string name, SyukatsuGame *game, Field *field)
  :EnemyCharacter(name, game, field)
{
  this->setAttributes(10, 10, 10, 10.0f, Vector3(0.0f, 1.0f, 0.0f)); 
}

void EnemySoldier::update(float deltaTime)
{  
}
