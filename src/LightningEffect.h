#ifndef LIGHTNING_EFFECT_H
#define LIGHTNING_EFFECT_H

#include "Effect.h"

class LightningEffect : public Effect
{
  LightningEffect(string name, SyukatsuGame *game, Vector3 position);
  ~LightningEffect();

  void update(float deltaTime);
  void render(float deltaTime);    
};

#endif
