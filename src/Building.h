#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "Actor.h"
#include "Field.h"
#include <math/Vector3.h>

class Building : public Actor
{
protected:
  Field* field;
  Vector3 position;

  int baseValue;


public:
  Building(std::string _name, SyukatsuGame *_game, Field *_field);
  virtual ~Building() {}

  virtual void render(float deltaTime);

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

};

#endif
