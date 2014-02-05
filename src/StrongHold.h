#ifndef STRONG_HOLD
#define STRONG_HOLD

#include "Actor.h"

class Field;
class StrongHold : public Actor
{
  static constexpr float invisibleTime = 1.0f;
  int health;
  float time; //ダメージを受けて無敵中の時間
  Field *field;
public:
  StrongHold(string name, SyukatsuGame *game, Field *_field);
  ~StrongHold();
  //包囲攻撃を受ける
  void siege();

  bool destroyed() const;

  void update(float deltaTime);
  
  void render(float deltaTime);

};

#endif
