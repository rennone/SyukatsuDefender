#ifndef _CHARACTOR_H_2013_12_25
#define _CHARACTOR_H_2013_12_25

#include "Actor.h"
#include "Field.h"

#include <math/Vector3.h>

//プレイシーンで動き回るキャラクター, フィールド, 位置, 方向, 速度を持つ
class Character : public Actor
{
protected:
  Vector3 position;
  Vector3 direction;
  float speed;
  Field *field;  
public:
  Character(std::string _name, SyukatsuGame *_game, Field *_field);
  virtual ~Character() {}
  
  virtual void render(float deltaTime);
    
  void setPosition(const float &x, const float &y, const float &z)
  {
    position.set(x,y,z);    
  }

  void setPosition(const Vector3 &pos)
  {
    position = pos;    
  }

  Vector3 getPosition() const
  {
    return position;    
  }

  void setDirection(const float &x, const float &y, const float &z)
  {
    direction.set(x,y,z);    
  }

  void setDirection(const Vector3 &dir)
  {
    direction = dir;
  }

  Vector3 getDirection() const
  {
    return direction;
  }    
  
};

#endif
