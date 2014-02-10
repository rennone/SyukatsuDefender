#include "Barrack.h"
#include "Soldier.h"
#include "Knight.h"
#include "HeavyArmor.h"
#include "Assets.h"
#include "Information.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

Barrack::Barrack(string _name, SyukatsuGame *_game, Field *_field, CharacterManager* _cmanager)
  :Building(_name, _game, _field), cmanager(_cmanager), timer(0), spawned(0)
{
  setModel(Assets::buildings[Information::LIGHTNING_TOWER]);
  
  for(int i = 0; i < 10; ++i) {
    spawnList.push_back(SpawnStatus( i % 2, 1, i % 3, 3 * i + 2));
  }			
}

void Barrack::update(float deltaTime)
{
  timer += deltaTime;
  if(spawned == spawnList.size()) {
    Actor::setStatus(Actor::Dead);
  }
  else {
    while(spawned < spawnList.size()) {
      if(spawnList[spawned].getTime() > timer) break;

      auto new_soldier = getInstanceOfCharacter(spawnList[spawned].getType());
	
      new_soldier->setPosition(position);
      new_soldier->setLane(spawnList[spawned].getLane());

      cmanager->addChild(new_soldier);
      spawned++;
    }
  }

  Building::update(deltaTime);
}

float Barrack::calcRange()
{
  return rangeOfEffect + 20 * (level - 1);
}

float Barrack::calcAttackRate() 
{
  return attackRate * (1.0 - 0.1 * (level - 1));
}

int Barrack::calcAttack()
{
  return attack + 10 * (level - 1);
}

Character* Barrack::getInstanceOfCharacter(int type)
{
  Character *character;
  switch(type) {
  case Information::SOLDIER:
    character = new Soldier("soldier", syukatsuGame, field);
    break;

  case Information::KNIGHT:
    character = new Knight("knight", syukatsuGame, field);
    break;

  case Information::HEAVYARMOR:
    character = new HeavyArmor("heavyarmor", syukatsuGame, field);
    break;

  default:
    assert(false);
    return NULL;
  }

  return character;
}


SpawnStatus::SpawnStatus(int _type, int _level, int _lane, float _time)
  :type(_type), level(_level), lane(_lane), time(_time)
{
}

bool SpawnStatus::operator<(const SpawnStatus& sStatus) const
{
  return time < sStatus.time;
}


