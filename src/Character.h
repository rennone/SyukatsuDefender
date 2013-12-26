#ifndef _CHARACTOR_H_2013_12_25
#define _CHARACTOR_H_2013_12_25

#include "Actor.h"
#include <math/Vector3.h>

class Character : public Actor
{
protected:
  Vector3 position;
  Vector3 direction;
  float speed;
  int maxhp;
  int hp;
  int attack;
  int defense;

  //temporary characteristic
  Vector3 color;

public:
  Character(std::string _name, SyukatsuGame *_game);
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

  void setAttributes(int _maxhp, int _attack, int _defense, float _speed, Vector3 _color) 
  {
      maxhp = _maxhp;
      hp = maxhp;
      attack = _attack;
      defense = _defense;
      speed = _speed;
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

  
};

#endif
