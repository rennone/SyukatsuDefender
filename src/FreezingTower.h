#ifndef __FREEZINGTOWER_H__
#define __FREEZINGTOWER_H__

#include "Building.h"
#include "CharacterManager.h"

const double slowRange[6] = {0, 50.0, 70.0, 100.0, 120.0, 150.0};

class FreezingTower : public Building
{
  protected:
    float timer;
    CharacterManager *cmanager;

  public:
    FreezingTower(string _name, SyukatsuGame *_game, Field *_field, CharacterManager* _cmanager);

    void update(float deltaTime);
    void render(float deltaTime);
    void upgrade();
};

#endif
