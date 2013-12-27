#include "PlayerBowman.h"
#include <random>
using namespace std;

//3秒ごとにランダムな方向へ, フィルドの外に出ようとしても方向を変える
void PlayerBowman::update(float deltaTime)
{
  elapsedTime += deltaTime;

  if(elapsedTime > 3)
  {
    elapsedTime = 0;
    direction = Vector3(rand()%10, 0, rand()%10);
    direction.normalize();    
  }
  
  if(field->collision(position, direction*deltaTime*speed, position))
  {
    //todo 初期化もしていない, メルセンヌツイスターを使っていない
    //direction = Vector3(rand()%10, rand()%10, rand()%10);
    //direction.normalize();    
  } 

  
  
}
