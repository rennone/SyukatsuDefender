#ifndef _CHARACTOR_H_2013_12_25
#define _CHARACTOR_H_2013_12_25

#include "Actor.h"
#include "Field.h"

#include <math/Vector3.h>

//プレイシーンで動き回るキャラクター, フィールド, 位置, 方向, 速度を持つ
class Character : public Actor
{
protected:
  Field *field;  
  Vector3 position;
  Vector3 direction;
  float speed;
  int maxhp;
  int hp;
  int attack;
  int defense;  
  float radius;  //衝突判定用の半径  
  Vector3 color;  //temporary characteristic

public:
  Character(std::string _name, SyukatsuGame *_game, Field *_field);
  virtual ~Character() {}
  
  virtual void render(float deltaTime);
  virtual void update(float deltaTime);
    
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

  void setAttributes(const int _maxhp, const int _attack, const int _defense, const float _speed, const Vector3 _color) 
  {
      maxhp = _maxhp;
      hp = maxhp;
      attack = _attack;
      defense = _defense;
      speed = _speed;
      color = _color;
  }

  void setColor(const Vector3 _color) 
  {
    color = _color;
  }

  int getCurrentHp() 
  {
      return hp;
  }

  int getMaxHp() 
  {
      return maxhp;
  }

  int getAttack() 
  {
      return attack;
  }

  int getDefense()
  {
      return defense;
  }

  int getRadius() const { return radius; }

  //倒される時trueを返す
  bool gotDamage(int value);
  
  bool isHit(const Character* c);

  
};

#endif
