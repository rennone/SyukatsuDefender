#ifndef ENEMY_CHARACTER_MANAGER_H_2013_12_26
#define ENEMY_CHARACTER_MANAGER_H_2013_12_26

#include "EnemyCharacter.h"

//全てのエネミーを管理するクラス
class EnemyCharacterManager:public EnemyCharacter
{
public:
EnemyCharacterManager(string name, SyukatsuGame *game, Field *field)
  :EnemyCharacter(name, game, field)
  {
    
  }
  
  ~EnemyCharacterManager() {  }
  
};

#endif
