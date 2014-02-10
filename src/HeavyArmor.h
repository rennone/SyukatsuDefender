#ifndef __HEAVYARMOR_H__
#define __HEAVYARMOR_H__

#include "Character.h"

class HeavyArmor : public Character
{
public:
  HeavyArmor(string name, SyukatsuGame *game, Field *field);  
  ~HeavyArmor() {}   

  void update(float deltaTime);
};


#endif
