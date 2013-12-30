#ifndef __LIGHTNINGTOWER_H__
#define __LIGHTNINGTOWER_H__

#include "Building.h"
#include "CharacterManager.h"

const float attackrate = 3.00;

class LightningTower : public Building
{
 protected:
  float timer;
  CharacterManager *cmanager;

 public:
  LightningTower(string _name, SyukatsuGame *_game, Field *_field, CharacterManager* _cmanager);
  ~LightningTower() {}

  void update(float deltaTime);
};

#endif
