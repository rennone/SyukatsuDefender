#include "Player.h"
#include "Debugger.h"
#include "Information.h"

Player::Player(string _name, SyukatsuGame *_game, int initialGold)
  :Actor(_name, _game), 
   gold(initialGold),
   mana(Information::InitialMana),
   manaregen(Information::InitialManaregen)
   
{
}

void Player::render(float deltaTime)
{
  Actor::render(deltaTime);
}
  
void Player::update(float deltaTime)
{
  mana += deltaTime * manaregen;
  mana = std::min(mana, InitialMana);

  Debugger::drawDebugInfo("Player.cpp", "Mana", mana);

  Actor::update(deltaTime);
}

bool Player::canMagicAttack() 
{
  return mana >= Information::FireballCost;
}

void Player::castFireball()
{
  mana -= Information::FireballCost;
}

