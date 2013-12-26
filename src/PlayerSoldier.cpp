#include "PlayerSoldier.h"

PlayerSoldier::PlayerSoldier(string name, SyukatsuGame *game)
  :PlayerCharacter(name, game)
{
  this->setAttributes(10, 10, 10, 10.0f, Vector3(1.0f, 0.0f, 0.0f)); 
}

void PlayerSoldier::update(float deltaTime)
{  
  if( position.distanceTo(distination) < speed*deltaTime )
  {
    setStatus(Actor::Dead); //たどり着いたら死ぬ    
    return;    
  }
  direction = distination - position;
  direction.normalize();
  
  position += direction*deltaTime*speed;  
}

