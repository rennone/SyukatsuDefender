#ifndef LIGHTNING_EFFECT_H
#define LIGHTNING_EFFECT_H

#include "Effect.h"

class LightningEffect : public Effect
{
  float range;
  float elapsedTime;
  float time;
  
public:
  LightningEffect(string name, SyukatsuGame *game);
  ~LightningEffect();

  void playEffect(Vector3 position, float time);  
  void update(float deltaTime);
  void render(float deltaTime);    
};

#endif
