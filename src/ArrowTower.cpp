#include "ArrowTower.h"
#include "Assets.h"
#include "Information.h"

ArrowTower::ArrowTower(string _name, SyukatsuGame *_game, Field *_field, CharacterManager *_cmanager)
  :Building(_name, _game, _field), cmanager(_cmanager), timer(0)
{
  setBaseValue(100);
  setAttackRate(1.20);
  setAttack(10);
  setRangeOfEffect(Information::DefaultRangeOfBuildings[Information::ARROW_TOWER]);
  setModel(Assets::buildings[Information::ARROW_TOWER]);
}

void ArrowTower::update(float deltaTime)
{
  timer += deltaTime;
  if(timer >= calcAttackRate()) {
    bool attacked = false;

    auto enemyList = cmanager->getChildren();
    Character* target = NULL;
    double mindist = calcRange();

    for(auto c : enemyList) {
      Vector3 dist = ((Character*)c)->getPosition() - position;
      if(dist.length() < mindist) {
	mindist = dist.length();
	
	target = (Character *)c;
      }
    }

    if(target != NULL) {
      target->gotDamage(calcAttack());
      attacked = true;
    }

    if(attacked) {
      timer = 0;
    }
  }

  Building::update(deltaTime);  
}

void ArrowTower::upgrade()
{
  const int maxLevel = 5;
  if(level < maxLevel) {
    level++;
  }
}

void ArrowTower::render(float deltaTime)
{
  Building::render(deltaTime);
}

float ArrowTower::calcRange()
{
  return rangeOfEffect + 20 * (level - 1);
}

float ArrowTower::calcAttackRate() 
{
  return attackRate * (1.0 - 0.1 * (level - 1));
}

int ArrowTower::calcAttack()
{
  return attack + 10 * (level - 1);
}

