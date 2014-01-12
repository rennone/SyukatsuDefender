#ifndef __FREEZINGTOWER_H__
#define __FREEZINGTOWER_H__

#include "Building.h"
#include "CharacterManager.h"


const float attackrate = 5.00;

class FreezingTower : public Building
{
  protected:
    float timer;
    CharacterManager *cmanager;

  public:
    FreezingTower(string _name, SyukatsuGame *_game, Field *_field, CharacterManager* _cmanager);

    void update(float deltaTime);
    void render(float deltaTime);
};

#endif
