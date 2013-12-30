#include "CharacterManager.h"

CharacterManager::CharacterManager(string _name, SyukatsuGame *_game, Field *_field)
  :Character(_name, _game, _field)
{
  gold = 100;
}

void CharacterManager::update(float deltaTime)
{
  Actor::update(deltaTime);
}
