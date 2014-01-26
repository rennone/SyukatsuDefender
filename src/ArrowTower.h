#ifndef __ARROWTOWER_H__
#define __ARROWTOWER_H__

#include "Building.h"
#include "CharacterManager.h"

//範囲内でもっとも近い単体に攻撃するタワー

class ArrowTower : public Building
{
  protected:
    float timer;
    CharacterManager *cmanager;

  public:
    ArrowTower(string _name, SyukatsuGame *_game, Field *_field, CharacterManager* _cmanager);

    void update(float deltaTime);
    void render(float deltaTime);
    void upgrade();

    float calcRange();
    float calcAttackRate();
    int calcAttack();
};

#endif
