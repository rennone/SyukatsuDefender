#include "LightningTower.h"
#include "LightningEffect.h"
#include "Assets.h"
LightningTower::LightningTower(string _name, SyukatsuGame *_game, Field *_field, CharacterManager *_cmanager)
  :Building(_name, _game, _field), cmanager(_cmanager), timer(0)
{
  setBaseValue(100);
  setAttackRate(3.00);

  effect = new LightningEffect("effect", syukatsuGame);
  addChild(effect);  
}

void LightningTower::update(float deltaTime)
{
  timer += deltaTime;
  if(timer >= calcAttackRate()) {
    bool attacked = false;

    auto enemyList = cmanager->getChildren();
    for(auto c : enemyList) {
      Vector3 dist = ((Character*)c)->getPosition() - position;
      if(dist.length() < rangeOfEffect) {
	((Character*)c)->gotDamage(10000);
	attacked = true;
      }
    }

    if(attacked) {
      timer = 0;
      effect->playEffect(position, 1);        
    }
  }

  Building::update(deltaTime);  
}

void LightningTower::render(float deltaTime)
{
  Building::render(deltaTime);
}
