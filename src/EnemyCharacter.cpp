#include "EnemyCharacter.h"

EnemyCharacter::EnemyCharacter(string name, SyukatsuGame *game, Field* field)
  :Character(name, game, field)
{
  this->setAttributes(10, 10, 10, 10.0f, Vector3(1.0f, 0.0f, 0.0f)); 
}

