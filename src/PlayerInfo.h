#ifndef __PLAYERINFO_H__
#define __PLAYERINFO_H__

#include "Actor.h"
#include "Field.h"
#include <math/Vector3.h>
#include <syukatsu/SyukatsuGame.h>

class PlayerInfo
{
 protected:
  Actor* characterManager;
  Actor* buildingManager;

  Vector3 target;
  Vector3 teamColor;
  int gold;

 public:
  PlayerInfo(string _name, SyukatsuGame *_game);
  ~PlayerInfo() {}
  
  void setTarget(Vector3 _target) { target = _target; }
  Vector3 getTarget() const { return target; }

  void setColor(Vector3 _color) { teamColor = _color; }
  Vector3 getColor() const { return teamColor; }

  int getGold() { return gold; }
  void addGold(const int value) { gold += value; } 
  void subGold(const int value) { gold -= value; }
};

#endif
  
