#ifndef PLAYER_ATTACK_EFFECT_H
#define PLAYER_ATTACK_EFFECT_H

#include "LightningEffect.h"

class PlayerAttackEffect : public LightningEffect
{
public :
  PlayerAttackEffect(string name, SyukatsuGame *game)
    :LightningEffect(name, game)
  {
  }
  
  ~PlayerAttackEffect()
  {
  }
  
};


#endif
