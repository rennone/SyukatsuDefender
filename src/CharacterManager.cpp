#include "CharacterManager.h"

CharacterManager::CharacterManager(string _name, SyukatsuGame *_game, Field *_field)
  :Actor(_name, _game)
{
  gold = 100000;
}

void CharacterManager::update(float deltaTime)
{
  Actor::update(deltaTime);
}
