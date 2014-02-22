#ifndef PLAYER_SOLDIER_H_2013_12_26
#define PLAYER_SOLDIER_H_2013_12_26

#include "Character.h"
class Soldier : public Character
{
public:
    Soldier(string name, SyukatsuGame *game, Field *field, int _level);  
  ~Soldier() {  }   

  void update(float deltaTime);
};


#endif
