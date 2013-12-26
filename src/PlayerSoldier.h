#ifndef PLAYER_SOLDIER_H_2013_12_26
#define PLAYER_SOLDIER_H_2013_12_26

#include "PlayerCharacter.h"
class PlayerSoldier:public PlayerCharacter
{
  
public:
  PlayerSoldier(string name, SyukatsuGame *game);  
  ~PlayerSoldier() {  }   

  void update(float deltaTime);
};


#endif
