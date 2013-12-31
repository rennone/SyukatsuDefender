#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "Actor.h"
#include "Character.h"
#include "Field.h"
#include <math/Vector3.h>

class Building : public Actor
{
protected:
  Field* field;
  Vector3 position;

  int maxhp;
  int hp;

  int baseValue;
  bool picked;

  const float radius;  
public:
  Building(std::string _name, SyukatsuGame *_game, Field *_field);
  virtual ~Building() {}

  virtual void render(float deltaTime);
  virtual void update(float deltaTime);

  void setPosition(const float &x, const float &y, const float &z)
  {
    position.set(x, y, z);
  }

  void setPosition(const Vector3& pos)
  {
    position = pos;
  }

  Vector3 getPosition() const
  {
    return position;
  }

  void setBaseValue(const int value) { baseValue = value; }
  int getBaseValue() const { return baseValue; }

  void setPicked(const bool value) { picked = value; }
  bool getPicked() const { return picked; }

  float getRadius() const 
  {
    return radius;
  }
  
  bool collisionCheck(const Vector3 &before, const Vector3 &after, const Character *chara, Vector3 &collisionPos, Vector3 &normal) const;  

  void setMaxhp(const int value) { maxhp = value; }
  
  int getMaxhp() const { return maxhp; }

  void setAttributes(int _maxhp) {
    maxhp = _maxhp;
    hp = _maxhp;
  }

  bool gotDamage(int value);
};

#endif
