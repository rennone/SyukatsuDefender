#ifndef FREEZING_EFFECT_H
#include "Effect.h"

class Character;
class FreezingEffect : public Effect
{
  Character *target;
  float rot;
public:
  FreezingEffect(string name, SyukatsuGame *game);
  ~FreezingEffect();
  void render(float deltaTime);
  void update(float deltaTime);
  void set(Character *target)
  {
    this->target = target;
    setStatus(Actor::Action);
    rot = 0;
  }
};

#endif
