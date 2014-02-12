#ifndef LIGHTNING_EFFECT_H
#define LIGHTNING_EFFECT_H

#include "Effect.h"

class Character;
class LightningEffect : public Effect
{
  float range;
  float elapsedTime;
  float time;
public:
  LightningEffect(string name, SyukatsuGame *game);
  ~LightningEffect();

  virtual void playEffect(Vector3 position, float time);  
  virtual void update(float deltaTime);
  virtual void render(float deltaTime);
};

#endif
