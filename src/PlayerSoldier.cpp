#include "PlayerSoldier.h"

PlayerSoldier::PlayerSoldier(string name, SyukatsuGame *game)
  :PlayerCharacter(name, game)
{
  speed = 10;  
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

