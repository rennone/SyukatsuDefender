#include "PlayerSoldier.h"

PlayerSoldier::PlayerSoldier(string name, SyukatsuGame *game, Field *field)
  :PlayerCharacter(name, game, field)
{
  speed = 50;  
}

#include <iostream>
using namespace std;

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

