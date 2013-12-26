#include "PlayerSoldier.h"

PlayerSoldier::PlayerSoldier(string name, SyukatsuGame *game, Field *field)
  :PlayerCharacter(name, game, field)
{
  this->setAttributes(10, 10, 10, 10.0f, Vector3(1.0f, 0.0f, 0.0f)); 
}

void PlayerSoldier::update(float deltaTime)
{  
  if( position.distanceTo(destination) < speed*deltaTime )
  {
    setStatus(Actor::Dead); //たどり着いたら死ぬ    
    return;    
  }
  direction = destination - position;
  direction.normalize();
  
  position += direction*deltaTime*speed;  
}

