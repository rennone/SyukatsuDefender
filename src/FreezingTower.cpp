#include "FreezingTower.h"
#include "Assets.h"
#include "Information.h"

FreezingTower::FreezingTower(string _name, SyukatsuGame *_game, Field *_field, CharacterManager *_cmanager)
  :Building(_name, _game, _field), cmanager(_cmanager), timer(0)
{
  setBaseValue(200);
  setAttackRate(5.00);
  setAttack(10);
  setRangeOfEffect(Information::DefaultRangeOfBuildings[Information::FREEZING_TOWER]);
}

void FreezingTower::update(float deltaTime)
{
  timer += deltaTime;
  if(timer >= calcAttackRate()) {
    bool attacked = false;

    auto enemyList = cmanager->getChildren();
    for(auto c : enemyList) {
      Vector3 dist = ((Character*)c)->getPosition() - position;
      if(dist.length() < 70.0) {
	attacked = true;
	((Character *)c)->gotDamage(attack);
	((Character *)c)->gotFrozen();
      }
    }

    if(attacked) {
      timer = 0;
    }
  }

  Building::update(deltaTime);  
}

void FreezingTower::upgrade()
{
  const int maxLevel = 5;
  if(level < maxLevel) {
    level++;
    
    rangeOfEffect = slowRange[level];
  }
}

void FreezingTower::render(float deltaTime)
{
  Building::render(deltaTime);
}
