#ifndef PLAYER_CHARACTER_H_2013_12_26
#define PLAYER_CHARACTER_H_2013_12_26

#include "Character.h"

class PlayerCharacter : public Character
{
protected:
  Vector3 distination;
  
public:
  PlayerCharacter(string name, SyukatsuGame *game)
    :Character(name, game)
    ,distination(position)
  {
    
  }
  
  ~PlayerCharacter() {  }

  void setDistination(const Vector3 &dist)
  {
    distination = dist;
  }
  
  Vector3 getDistination() const
  {
    return distination;
  }  
};

#endif
