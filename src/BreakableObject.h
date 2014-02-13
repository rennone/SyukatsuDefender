#ifndef __BREAKABLE_OBJECT__
#define __BREAKABLE_OBJECT__

#include "Building.h"
#include "CharacterManager.h"

class BreakableObject : public Building
{
public:
  BreakableObject(string name, SyukatsuGame *game, Field *_field);
  ~BreakableObject();

  void update(float deltaTime);
  void render(float deltaTime);

  void upgrade();
  
  float calcRange();
  float calcAttackRate();
  int getUpgradeCost();
  int calcAttack();
};

class BreakableTree : public BreakableObject
{
public:
    BreakableTree(string _name, SyukatsuGame *_game, Field *_field);
    ~BreakableTree();
};

#endif
