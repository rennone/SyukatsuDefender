#include "ArrowTower.h"
#include "Assets.h"
#include "Information.h"

ArrowTower::ArrowTower(string _name, SyukatsuGame *_game, Field *_field, CharacterManager *_cmanager)
  :Building(_name, _game, _field), cmanager(_cmanager), timer(0)
{
  buildingName = "ArrowTower";
  description = "attacking\nsingle enemy";

  //towerの能力値読み込み
  setAttributes(Information::ARROW_TOWER);

  setModel(Assets::buildings[Information::ARROW_TOWER]);

  effect = new ArrowEffect("arrow", syukatsuGame);
  addChild(effect);
}

ArrowTower::~ArrowTower()
{
  //addChildしているのでdeleteの必要ない
}

void ArrowTower::update(float deltaTime)
{
  timer += deltaTime;
  if(timer >= calcAttackRate())
  {  
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

    if(target != NULL && effect->getStatus() == Actor::NoUse)
    {
      effect->shoot(target, calcAttack(), calcAttackSpeed(), position);
      timer=0;
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

float ArrowTower::calcAttackSpeed()
{
  const float speed = 100;
  return speed*attackRate;
}

