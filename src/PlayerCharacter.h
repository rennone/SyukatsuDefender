#ifndef PLAYER_CHARACTER_H_2013_12_26
#define PLAYER_CHARACTER_H_2013_12_26

#include "Character.h"

class PlayerCharacter : public Character
{
protected:
  Vector3 destination;
  
public:
  PlayerCharacter(string name, SyukatsuGame *game, Field *field);
  
  ~PlayerCharacter() {  }

  void setDestination(const Vector3 &dest)
  {
    destination = dest;
  }
  
  Vector3 getDestination() const
  {
    return destination;
  }  
};

#endif
