#ifndef PLAYER_BOWMAN_H
#define PLAYER_BOWMAN_H

#include "Character.h"

class PlayerBowman : public Character
{
protected:
  float elapsedTime;
  
public:
PlayerBowman(string name, SyukatsuGame *game, Field *field)
  :Character(name, game, field)
    ,elapsedTime(0)
  {
    setAttributes(10, 10, 10, 100, Vector3(0,0,1));    
  }

  void update(float deltaTime); 
  
};



#endif
