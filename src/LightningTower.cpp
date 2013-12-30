#include "LightningTower.h"

LightningTower::LightningTower(string _name, SyukatsuGame *_game, Field *_field, CharacterManager *_cmanager)
  :Building(_name, _game, _field), cmanager(_cmanager), timer(0)
{
}

void LightningTower::update(float deltaTime)
{
  timer += deltaTime;
  if(timer >= attackrate) {
  }
}
