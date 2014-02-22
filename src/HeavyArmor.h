#ifndef __HEAVYARMOR_H__
#define __HEAVYARMOR_H__

#include "Character.h"

class HeavyArmor : public Character
{
public:
    HeavyArmor(string name, SyukatsuGame *game, Field *field, int level);  
  ~HeavyArmor() {}   

  void update(float deltaTime);
  
  int calcMaxhp(int baseHp);
};


#endif
