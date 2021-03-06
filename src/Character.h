#ifndef _CHARACTOR_H_2013_12_25
#define _CHARACTOR_H_2013_12_25

#include <syukatsu/syukatsu.h>
#include "Actor.h"
#include "Field.h"
#include "Building.h"
#include "Collider.h"
#include <math/Vector3.h>

class Building;

//プレイシーンで動き回るキャラクター, フィールド, 位置, 方向, 速度を持つ
class Character : public Actor
{
protected:
  Field *field;  
  Vector3 position;
  Vector3 direction;
  Vector3 destination;
  vector<pair<int, int> > destinations;
  int curdst;
  int gold;

  int lane;
  float speed;
  int maxhp;
  int hp;
  int attack;
  int defense;
  int level;
  float radius;  //衝突判定用の半径  
  Vector3 color;  //temporary characteristic

  Model *model;

  //速度低下関連
  bool froze;
  double duration;
  
public:
  Character(std::string _name, SyukatsuGame *_game, Field *_field, int _level);
  virtual ~Character() {}
  
  virtual void render(float deltaTime);
  virtual void update(float deltaTime);

  virtual int calcMaxhp(int baseHp) = 0;
    
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

  void setDestination(const Vector3 &dest)
  {
    destination = dest;
  }
  
  Vector3 getDestination() const
  {
    return destination;
  }

  void setAttributes(int type); 

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

  int getRadius() const
  {
    return radius;
  }

  bool isFrozen() const
  {
    return froze;
  }
  
  //倒される時trueを返す
  bool gotDamage(int value);  
  bool isHit(const Character* c);

  void gotFrozen();

  //コリジョンチェック
  bool collisionCheck(const Vector3 &before, const Vector3 &after, const Character *chara, Vector3 &collisionPos, Vector3 &normal) const;
    //コリジョンチェック
  bool collisionCheck(const Vector3 &before, const Vector3 &after, const Building *chara, Vector3 &collisionPos, Vector3 &normal) const;

    //コリジョンチェック
  bool collisionCheck(const Vector2 &before, const Vector2 &after, const Character *chara, Vector2 &collisionPos, Vector2 &normal) const;

  void setModel(Model *_model) { model = _model; }

  void setLane(int lane);
  int getGold() { return gold; }
  
};

#endif
