#include "BreakableObject.h"
#include "Assets.h"
#include "Information.h"

BreakableObject::BreakableObject(string _name, SyukatsuGame *_game, Field *_field)
  :Building(_name, _game, _field)
{
}

BreakableObject::~BreakableObject()
{
}

void BreakableObject::update(float deltaTime)
{  
  Building::update(deltaTime);
}

void BreakableObject::render(float deltaTime)
{
  Building::render(deltaTime);
}

void BreakableObject::upgrade()
{
  const int maxlevel = 1;
  if(level < maxlevel) {
    ++level;
  }
}

int BreakableObject::getUpgradeCost() 
{
  return 0;
}

float BreakableObject::calcRange()
{
  return 0;
}

float BreakableObject::calcAttackRate()
{
  return 0;
}

int BreakableObject::calcAttack()
{
  return 0;
}

BreakableTree::BreakableTree(string _name, SyukatsuGame *_game, Field *_field)
  :BreakableObject(_name, _game, _field)
{
  //towerの能力値読み込み
  //setAttributes(Information::BREAKABLE_TREE);

  //setModel(Assets::buildings[Information::BREAKABLE_TREE]);
}

BreakableTree::~BreakableTree()
{
}
