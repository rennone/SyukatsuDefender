#ifndef ENEMY_CHARACTER_H_2013_12_26
#define ENEMY_CHARACTER_H_2013_12_26

#include "Character.h"

class EnemyCharacter: public Character
{
protected:
    Vector3 destination;
public:
  EnemyCharacter(string name, SyukatsuGame *game, Field *field);
  ~EnemyCharacter() {  }    

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
