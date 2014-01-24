#include "ArrowTower.h"
#include "Assets.h"

ArrowTower::ArrowTower(string _name, SyukatsuGame *_game, Field *_field, CharacterManager *_cmanager)
  :Building(_name, _game, _field), cmanager(_cmanager), timer(0)
{
  setBaseValue(100);
  setAttackRate(1.20);
  setAttack(10);
  setRangeOfEffect(100.0);
}

void ArrowTower::update(float deltaTime)
{
  timer += deltaTime;
  if(timer >= calcAttackRate()) {
    bool attacked = false;

    auto enemyList = cmanager->getChildren();
    Character* target = NULL;
    double mindist = rangeOfEffect;

    for(auto c : enemyList) {
      Vector3 dist = ((Character*)c)->getPosition() - position;
      if(dist.length() < mindist) {
	mindist = dist.length();
	
	target = (Character *)c;
      }
    }

    if(target != NULL) {
      target->gotDamage(attack);
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
