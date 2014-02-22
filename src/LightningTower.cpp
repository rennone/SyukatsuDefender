#include "LightningTower.h"
#include "LightningEffect.h"
#include "Assets.h"
#include "Information.h"

LightningTower::LightningTower(string _name, SyukatsuGame *_game, Field *_field, CharacterManager *_cmanager)
  :Building(_name, _game, _field), cmanager(_cmanager), timer(0)
{
  buildingName = "LightningTower";
  description = "attacking\nmultiple enemy";

  //towerの能力値読み込み
  setAttributes(Information::LIGHTNING_TOWER);

  setModel(Assets::buildings[Information::LIGHTNING_TOWER]);
  effect = new LightningEffect("effect", syukatsuGame);

  addChild(effect);  
}

LightningTower::~LightningTower()
{
  //addChildしているので, deleteの必要は無い
}

void LightningTower::update(float deltaTime)
{
  timer += deltaTime;
  if(timer >= calcAttackRate()) {
    bool attacked = false;

    auto enemyList = cmanager->getChildren();
    Character *tage = NULL;
    for(auto c : enemyList) {
      Vector3 dist = ((Character*)c)->getPosition() - position;
      if(dist.length() < calcRange())
      {
	((Character*)c)->gotDamage(calcAttack());
	attacked = true;
        tage = ((Character*)c);      
      }
    }

    if(attacked) {
      timer = 0;
      effect->playEffect( tage->getPosition(), 0.5);
    }
  }

  Building::update(deltaTime);  
}

void LightningTower::render(float deltaTime)
{
  Building::render(deltaTime);
}

float LightningTower::calcRange()
{
  return rangeOfEffect + 20 * (level - 1);
}

float LightningTower::calcAttackRate() 
{
  return attackRate * (1.0 - 0.1 * (level - 1));
}

int LightningTower::calcAttack()
{
  return attack + 10 * (level - 1);
}
