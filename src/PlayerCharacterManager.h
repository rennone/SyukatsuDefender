#ifndef PLAYER_CHARACTER_MANAGER_H_2013_12_26
#define PLAYER_CHARACTER_MANAGER_H_2013_12_26

#include "PlayerCharacter.h"
#include "Field.h"
#include <syukatsu/syukatsu.h>
//全てのプレイヤーを管理するクラス
class PlayerCharacterManager:public PlayerCharacter
{
  Camera3D *camera;
Field *field;
public:
PlayerCharacterManager(string name, SyukatsuGame *game, Camera3D *cam, Field *_field)
    :PlayerCharacter(name, game)
    ,camera(cam)
  ,field(_field)
  {
  }
  
  ~PlayerCharacterManager() {  }
  void update(float deltaTime);  
};

#endif
