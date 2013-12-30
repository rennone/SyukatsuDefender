#ifndef __BARRACK_H__
#define __BARRACK_H__

#include "Building.h"
#include "CharacterManager.h"

const float spawnrate = 2.00;

class Barrack : public Building
{
 protected:
  float timer;
  CharacterManager* cmanager;

 public:
  Barrack(string _name, SyukatsuGame *_game, Field *_field, CharacterManager* _cmanager);
  ~Barrack() {};

  void update(float deltaTime);
};

#endif
