#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Actor.h"
#include "Field.h"
#include <math/Vector3.h>
#include <syukatsu/SyukatsuGame.h>

class Player : public Actor
{
 protected:
  Actor* buildingManager;

  Vector3 teamColor;
  int gold;
  int mana;

 public:
  Player(string _name, SyukatsuGame *_game, int initialGold);
  ~Player() {}

  void setColor(Vector3 _color) { teamColor = _color; }
  Vector3 getColor() const { return teamColor; }

  int getGold() { return gold; }
  void addGold(const int value) { gold += value; } 
  void subGold(const int value) { gold -= value; }

  void render(float deltaTime);
  void update(float deltaTime);

  
};

#endif
  
