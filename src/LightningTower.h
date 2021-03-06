#ifndef __LIGHTNINGTOWER_H__
#define __LIGHTNINGTOWER_H__

#include "Building.h"
#include "CharacterManager.h"
#include "LightningEffect.h"

class LightningTower : public Building
{
 protected:
  float timer;
  CharacterManager *cmanager;
  LightningEffect *effect;  

 public:
  static constexpr float defaultRangeOfEffect = 70.0;  
  LightningTower(string _name, SyukatsuGame *_game, Field *_field, CharacterManager* _cmanager);
  ~LightningTower();

  void update(float deltaTime);
  void render(float deltaTime);

  float calcRange();
  float calcAttackRate();
  int calcAttack();
};

#endif
