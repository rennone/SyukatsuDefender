#include "Player.h"

Player::Player(string _name, SyukatsuGame *_game, int initialGold)
  :Actor(_name, _game), 
   gold(initialGold)
{
  buildingManager = new Actor("bbb", _game);
}

void Player::render(float deltaTime)
{
  Actor::render(deltaTime);
}
  
void Player::update(float deltaTime)
{

  Actor::update(deltaTime);
}
