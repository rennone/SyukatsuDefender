#ifndef ARROW_H
#include "Effect.h"

class Character;
class ArrowEffect : public Effect
{
  Character *target;
  float attack;
  float speed;
public:
  ArrowEffect(string name, SyukatsuGame *game);
  ~ArrowEffect();

  void setTarget(Character *target)
  {
    this->target = target;
  }

  Character* getTarget() const
  {
    return target;
  }

  void shoot(Character *target, float attack, float speed, Vector3 position)
  {
    this->target = target;
    this->attack = attack;
    this->speed = speed;
    this->position = position;
    setStatus(Actor::Action);
  }
  
  void update(float deltaTime);
  void render(float deltaTime);
};

#endif
