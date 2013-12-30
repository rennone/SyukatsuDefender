#ifndef PLAYER_SOLDIER_H_2013_12_26
#define PLAYER_SOLDIER_H_2013_12_26

#include "Character.h"
class PlayerSoldier : public Character
{
  
public:
  PlayerSoldier(string name, SyukatsuGame *game, Field *field);  
  ~PlayerSoldier() {  }   

  void update(float deltaTime);
};


#endif
