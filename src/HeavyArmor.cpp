#include "HeavyArmor.h"
#include "Information.h"
#include "Assets.h"

HeavyArmor::HeavyArmor(string name, SyukatsuGame *game, Field *field)
  :Character(name, game, field)
{
  setAttributes(Information::Enemies::HEAVYARMOR); 
  setModel(Assets::enemies[Information::HEAVYARMOR]);
}

void HeavyArmor::update(float deltaTime)
{
  Character::update(deltaTime);
}

