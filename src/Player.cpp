#include "Player.h"
#include "Debugger.h"
#include "Information.h"

Player::Player(string _name, SyukatsuGame *_game, int initialGold)
  :Actor(_name, _game), 
   gold(initialGold),
   mana(Information::InitialMana),
   manaregen(Information::InitialManaregen)
   
{
  effect = new PlayerAttackEffect("playerEffect", _game);
  effect->setStatus(Actor::NoUse);
  addChild(effect);
}

void Player::render(float deltaTime)
{
  mana += deltaTime * manaregen;
  mana = std::min(mana, InitialMana);

  #ifdef DEBUG
  Debugger::drawDebugInfo("Player.cpp", "Mana", mana);
  #endif
  
  Actor::render(deltaTime);
}
  
void Player::update(float deltaTime)
{
  Actor::update(deltaTime);
}

bool Player::canMagicAttack() 
{
  return mana >= Information::FireballCost;
}

void Player::castFireball(const Vector3 pos)
{
  mana -= Information::FireballCost;

  if (effect->getStatus() == Actor::NoUse )
    effect->playEffect( pos, 0.5);
}

