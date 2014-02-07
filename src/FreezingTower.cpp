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

  for(int i=0; i<maxEffect; i++)
  {
    effectPool[i] = new FreezingEffect("freezingEffect", syukatsuGame);
    effectPool[i]->setStatus(Actor::NoUse);
  }
}

FreezingTower::~FreezingTower()
{
  //addChildしているので, deleteの必要は無い
}

FreezingEffect * FreezingTower::newEffect()
{
  for(auto effect : effectPool)
  {
    if(effect->getStatus() != Actor::NoUse)
      continue;

    return effect;
  }
  
  return NULL;
}

void FreezingTower::update(float deltaTime)
{
  timer += deltaTime;

  //先に計算しないと, 同じ的に2重にエッフェクとしてしまうのでaddChildせずに明示的にupdate
  for(auto effect : effectPool)
  {
    if(effect->getStatus() == Actor::NoUse)
      continue;
    
    effect->update(deltaTime);
  }
  
  if(timer >= calcAttackRate())
  {
    auto enemyList = cmanager->getChildren();
    for(auto c : enemyList)
    {
      Vector3 dist = ((Character*)c)->getPosition() - position;
      
      if(dist.length() < calcRange() && !((Character*)c)->isFrozen() )
      {
	((Character *)c)->gotDamage(calcAttack());
	((Character *)c)->gotFrozen();

        auto effect = newEffect();
        if(effect != NULL)
          effect->set((Character*)c);
        else
          cout << "NULL" << endl;

        timer = 0;
        break;
      }
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
  for(auto effect : effectPool)
  {
    if(effect->getStatus() == Actor::NoUse)
      continue;
    
    effect->render(deltaTime);
  }
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

