#include "PlayerBowman.h"
#include <random>
#include "Debugger.h"
using namespace std;

//3秒ごとにランダムな方向へ, フィルドの外に出ようとしても方向を変える
void PlayerBowman::update(float deltaTime)
{
  elapsedTime += deltaTime;

  if(position.distanceTo(destination) < deltaTime*speed*10)
    setStatus(Actor::Dead);
  
  direction = destination - position;
  direction.normalize();  
  Vector3 after = position + direction*deltaTime*speed;
  position = after;
  Debugger::drawDebugInfo("PlayerBowman.cpp", "position", position);  
}
