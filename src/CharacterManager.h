#ifndef __CHARACTER_MANAGER_H__
#define __CHARACTER_MANAGER_H__

#include "Character.h"
#include "Field.h"
#include <syukatsu/syukatsu.h>
#include <math/Vector3.h>

class CharacterManager : public Character
{
 protected:
  Vector3 target;
  Vector3 color;
  int gold;

 public:
  CharacterManager(string _name, SyukatsuGame *_game, Field *_field);

  ~CharacterManager() {}

  void update(float deltaTime);

  void setTarget(Vector3 _target) { target = _target; }
  Vector3 getTarget() const { return target; }

  void setColor(Vector3 _color) { color = _color; }
  Vector3 getColor() const { return color; }

  int getGold() { return gold; }
  void addGold(const int value) { gold += value; } 
  void subGold(const int value) { gold -= value; }
};

#endif
