#include "CharacterManager.h"

CharacterManager::CharacterManager(string _name, SyukatsuGame *_game, Field *_field)
  :PlayerCharacter(_name, _game, _field)
{
  
}

void CharacterManager::update(float deltaTime)
{
  Actor::update(deltaTime);
}
