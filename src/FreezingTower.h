#ifndef __FREEZINGTOWER_H__
#define __FREEZINGTOWER_H__

#include "Building.h"
#include "CharacterManager.h"
#include "FreezingEffect.h"

class FreezingTower : public Building
{
  static constexpr int maxEffect = 20;
  FreezingEffect *effectPool[maxEffect];
  FreezingEffect* newEffect();

  protected:
    float timer;
    CharacterManager *cmanager;
    
  public:
    FreezingTower(string _name, SyukatsuGame *_game, Field *_field, CharacterManager* _cmanager);
    ~FreezingTower();
    void update(float deltaTime);
    void render(float deltaTime);
    void upgrade();

    float calcRange();
    float calcAttackRate();
    int calcAttack();
};

#endif
