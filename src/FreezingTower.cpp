#include "FreezingTower.h"
#include "Assets.h"

FreezingTower::FreezingTower(string _name, SyukatsuGame *_game, Field *_field, CharacterManager *_cmanager)
  :Building(_name, _game, _field), cmanager(_cmanager), timer(0)
{
}

void FreezingTower::update(float deltaTime)
{
  timer += deltaTime;
  if(timer >= attackrate) {
    bool attacked = false;

    auto enemyList = cmanager->getChildren();
    for(auto c : enemyList) {
      Vector3 dist = ((Character*)c)->getPosition() - position;
      if(dist.length() < 70.0) {
	attacked = true;
      }
    }

    if(attacked) {
      timer = 0;
    }
  }

  Building::update(deltaTime);  
}

void LightningTower::render(float deltaTime)
{
  Building::render(deltaTime);
}
