#ifndef __BARRACK_H__
#define __BARRACK_H__

#include "Building.h"
#include "CharacterManager.h"
#include <vector>
#include <algorithm>

class SpawnStatus
{
  protected:
    int type; //敵の種類
    int lane; //通る道ID
    float time; //湧く時間
    int level; //敵レベル

  public:
    SpawnStatus() {}
    SpawnStatus(int _type, int _level, int _lane, float _time);

    bool operator<(const SpawnStatus& sStatus) const;
    int getType() { return type; }
    int getLane() { return lane; }
    float getTime() { return time; }
};

class Barrack : public Building
{
 protected:
  float timer;
  CharacterManager* cmanager;

  int spawned;
  vector<SpawnStatus> spawnList;

 public:
  Barrack(string _name, SyukatsuGame *_game, Field *_field, CharacterManager* _cmanager);
  ~Barrack() {};

  void update(float deltaTime);

  float calcRange();
  float calcAttackRate();
  int calcAttack();

  Character* getInstanceOfCharacter(int type);

};


#endif
