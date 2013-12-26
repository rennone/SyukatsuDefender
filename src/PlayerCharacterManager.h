#ifndef PLAYER_CHARACTER_MANAGER_H_2013_12_26
#define PLAYER_CHARACTER_MANAGER_H_2013_12_26

#include "PlayerCharacter.h"
#include "Field.h"
#include <syukatsu/syukatsu.h>
//全てのプレイヤーを管理するクラス
class PlayerCharacterManager:public PlayerCharacter
{
  Camera3D *camera;
public:
PlayerCharacterManager(string name, SyukatsuGame *game, Camera3D *cam, Field *field)
:PlayerCharacter(name, game, field)
    ,camera(cam)
  {
  }
  
  ~PlayerCharacterManager() {  }
  void update(float deltaTime);  
};

#endif
