#include "PlayerSoldier.h"

PlayerSoldier::PlayerSoldier(string name, SyukatsuGame *game, Field *field)
  :PlayerCharacter(name, game, field)
{
  this->setAttributes(50, 10, 10, 50.0f, Vector3(1.0f, 0.0f, 0.0f)); 
}

void PlayerSoldier::update(float deltaTime)
{
  Vector2 p(position.x, position.z), d(destination.x, destination.z);
  
  if( p.distanceTo(d) < speed*deltaTime )
  {
    setStatus(Actor::Dead); //たどり着いたら死ぬ    
    return;    
  }

  auto dir = d - p;  
  dir.normalize();

  Vector2 move = dir*deltaTime*speed;
  Vector3 after = position + Vector3(move.x, 0, move.y);  
  
  field->collision(position, after, radius); 
  position = after;

  Character::update(deltaTime);
}

