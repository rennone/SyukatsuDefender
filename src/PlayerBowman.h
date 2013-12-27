#ifndef PLAYER_BOWMAN_H
#define PLAYER_BOWMAN_H

#include "PlayerCharacter.h"

class PlayerBowman:public PlayerCharacter
{
  float elapsedTime;  
public:
PlayerBowman(string name, SyukatsuGame *game, Field *field)
  :PlayerCharacter(name, game, field)
    ,elapsedTime(0)
  {
    setAttributes(10, 10, 10, 100, Vector3(0,0,1));    
  }

  void update(float deltaTime); 
  
};



#endif
