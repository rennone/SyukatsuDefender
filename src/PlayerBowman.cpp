#include "PlayerBowman.h"
#include <random>
#include "Debugger.h"
using namespace std;

//3秒ごとにランダムな方向へ, フィルドの外に出ようとしても方向を変える
void PlayerBowman::update(float deltaTime)
{
  elapsedTime += deltaTime;

  if(elapsedTime > 1)
  {
    elapsedTime = 0;
    direction = Vector3(rand()%10, 0, rand()%10);
    direction.normalize();    
  }

  Vector3 after = position + direction*deltaTime*speed;

  field->collision(position, after, 10);
  position = after;
  Debugger::drawDebugInfo("PlayerBowman.cpp", "position", position);  
}
