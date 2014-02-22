#include "HeavyArmor.h"
#include "Information.h"
#include "Assets.h"

HeavyArmor::HeavyArmor(string name, SyukatsuGame *game, Field *field, int _level)
  :Character(name, game, field, _level)
{
  setAttributes(Information::Enemies::HEAVYARMOR); 
  setModel(Assets::enemies[Information::HEAVYARMOR]);
}

void HeavyArmor::update(float deltaTime)
{
  Character::update(deltaTime);
}

