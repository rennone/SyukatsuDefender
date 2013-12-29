#include "PlayerSoldier.h"

PlayerSoldier::PlayerSoldier(string name, SyukatsuGame *game, Field *field)
  :PlayerCharacter(name, game, field)
{
  this->setAttributes(10, 10, 10, 50.0f, Vector3(1.0f, 0.0f, 0.0f)); 
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

  Vector3 after = position + direction*deltaTime*speed;
  
  field->collision(position, after, 1); 
  position = after;   
}

