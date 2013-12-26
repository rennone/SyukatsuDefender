#ifndef ENEMY_SOLDIER_H_
#define ENEMY_SOLDIER_H_

#include "EnemyCharacter.h"
class EnemySoldier:public EnemyCharacter
{
  
public:
  EnemySoldier(string name, SyukatsuGame *game, Field *field);  
  ~EnemySoldier() {  }   

  void update(float deltaTime);
};


#endif
