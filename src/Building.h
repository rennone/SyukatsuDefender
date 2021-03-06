#ifndef __BUILDING_H__
#define __BUILDING_H__

#include <syukatsu/syukatsu.h>
#include "Actor.h"
#include "Character.h"
#include "Field.h"
#include "Collider.h"
#include <math/Vector3.h>
#include <string>

class Character;

class Building : public Actor
{
protected:
  Field* field;
  Vector3 position;
  std::string buildingName;
  std::string description;

  //parameters
  int maxhp;
  int hp;
  int maxlevel;
  int level;
  int attack;
  int baseValue;
  float rangeOfEffect; //効果範囲    
  float attackRate;

  bool picked;
  bool fixed; //仮建設状態かを表す

  const float radius;

  Model *model;

  CircleCollider* collider;

public:
  Building(std::string _name, SyukatsuGame *_game, Field *_field);
  virtual ~Building() {}

  virtual void render(float deltaTime);
  virtual void update(float deltaTime);

  virtual void upgrade();

  //能力計算用インターフェイス
  virtual float calcRange() = 0;
  virtual float calcAttackRate() = 0;
  virtual int calcAttack() = 0;

  virtual int getUpgradeCost();

  std::string getName() { return name; }
  std::string getDescription() { return description; }

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

  void setRangeOfEffect(float range)
  {
    rangeOfEffect = range;
  }

  float getRangeOfEffect() const
  {
    return rangeOfEffect;    
  }


  void setAttackRate(float value) { attackRate = value; }
  
  void setBaseValue(const int value) { baseValue = value; }
  int getBaseValue() { return baseValue; }

  void setPicked(const bool value) { picked = value; }
  bool getPicked() const { return picked; }

  float getRadius() const 
  {
    return radius;
  }
  
  bool collisionCheck(const Vector3 &before, const Vector3 &after, const Character *chara, Vector3 &collisionPos, Vector3 &normal) const;  

  void setMaxhp(const int value) { maxhp = value; }
  
  int getMaxhp() const { return maxhp; }

  void setAttributes(int type);

  int getLevel() { return level; }
  int getMaxLevel() { return maxlevel; }

  bool gotDamage(int value);
  bool isMaxLevel() { return level >= maxlevel; }
  int getSellValue();

  void setAttack(int atk) { attack = atk; }
  int getAttack() { return attack; }

  void setFixed(bool value) { fixed = value; }
  bool getFixed() { return fixed; }

  void drawTowerRange();

  void setModel(Model *_model) { model = _model; } 
  Model* getModel() { return model; }

  CircleCollider* getCollider() const
  {
    collider->position.set(position.x, position.z);    
    return collider;    
  }
  
};

#endif
