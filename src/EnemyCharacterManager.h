#ifndef ENEMY_CHARACTER_MANAGER_H_2013_12_26
#define ENEMY_CHARACTER_MANAGER_H_2013_12_26

//全てのエネミーを管理するクラス
class EnemyCharacterManager:public Actor
{
public:
  EnemyCharacterManager(string name, SyukatsuGame *game)
    :Actor(name, game)
  {
    
  }
  
  ~EnemyCharacterManager() {  }
  
};

#endif
