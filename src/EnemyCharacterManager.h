#ifndef ENEMY_CHARACTER_MANAGER_H_2013_12_26
#define ENEMY_CHARACTER_MANAGER_H_2013_12_26

#include "EnemyCharacter.h"
#include "Field.h"
#include <syukatsu/syukatsu.h>

//全てのエネミーを管理するクラス
class EnemyCharacterManager:public EnemyCharacter
{
private:
    Camera3D *camera;
public:
  EnemyCharacterManager(string name, SyukatsuGame *game, Camera3D *cam, Field *field)
      :EnemyCharacter(name, game, field), camera(cam)
  {
  }  
  ~EnemyCharacterManager() {  }
  void update(float deltaTime);
  
};

#endif
