#include "Barrack.h"
#include "Soldier.h"
#include "Knight.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

Barrack::Barrack(string _name, SyukatsuGame *_game, Field *_field, CharacterManager* _cmanager)
  :Building(_name, _game, _field), cmanager(_cmanager), timer(0), spawned(0)
{
  for(int i = 0; i < 10; ++i) {
    spawnList.push_back(SpawnStatus( i % 2, i % 3, 3 * i + 2));
  }			
}

void Barrack::update(float deltaTime)
{
  timer += deltaTime;
  while(spawned < spawnList.size()) {
    if(spawnList[spawned].getTime() > timer) break;

    if(spawnList[spawned].getType() == 0) {
      auto new_soldier = new PlayerSoldier("soldier", syukatsuGame, field);
      new_soldier->setPosition(position);
      new_soldier->setLane(spawnList[spawned].getLane());
      new_soldier->setColor(cmanager->getColor());

      cmanager->addChild(new_soldier);
    }
    else {
      auto new_soldier = new Knight("soldier", syukatsuGame, field);
      new_soldier->setPosition(position);
      new_soldier->setLane(spawnList[spawned].getLane());
      new_soldier->setColor(cmanager->getColor());

      cmanager->addChild(new_soldier);
    }

    spawned++;
  }

  Building::update(deltaTime);
}

SpawnStatus::SpawnStatus(int _type, int _lane, float _time)
  :type(_type), lane(_lane), time(_time)
{
}
  

bool SpawnStatus::operator<(const SpawnStatus& sStatus) const
{
  return time < sStatus.time;
}
