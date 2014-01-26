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
  setModel(Assets::buildings[Information::FREEZING_TOWER]);
}

void FreezingTower::update(float deltaTime)
{
  timer += deltaTime;
  if(timer >= calcAttackRate()) {
    bool attacked = false;

    auto enemyList = cmanager->getChildren();
    for(auto c : enemyList) {
      Vector3 dist = ((Character*)c)->getPosition() - position;
      if(dist.length() < calcRange()) {
	attacked = true;
	((Character *)c)->gotDamage(calcAttack());
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
  }
}

void FreezingTower::render(float deltaTime)
{
  Building::render(deltaTime);
}

float FreezingTower::calcRange()
{
  return rangeOfEffect + 20 * (level - 1);
}

float FreezingTower::calcAttackRate() 
{
  return attackRate * (1.0 - 0.1 * (level - 1));
}

int FreezingTower::calcAttack()
{
  return attack + 10 * (level - 1);
}

