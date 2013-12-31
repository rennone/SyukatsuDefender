#include "Barrack.h"
#include "PlayerSoldier.h"
#include <iostream>
using namespace std;

Barrack::Barrack(string _name, SyukatsuGame *_game, Field *_field, CharacterManager* _cmanager)
  :Building(_name, _game, _field), cmanager(_cmanager), timer(0)
{
}

void Barrack::update(float deltaTime)
{
  timer += deltaTime;
  if(timer >= spawnrate) {
    timer = 0;

    auto new_soldier = new PlayerSoldier("soldier", syukatsuGame, field);
    new_soldier->setPosition(position);
    new_soldier->setDestination(cmanager->getTarget());
    new_soldier->setColor(cmanager->getColor());

    cmanager->addChild(new_soldier);
  }

  Building::update(deltaTime);
}
