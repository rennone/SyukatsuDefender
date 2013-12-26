#include "PlayerCharacterManager.h"
#include "PlayerSoldier.h"

#include <iostream>
using namespace std;

void PlayerCharacterManager::update(float deltaTime)
{
  auto mouseEvent = syukatsuGame->getInput()->getMouseEvent();

  Vector3 point;
  if(mouseEvent->action == GLFW_PRESS)
  {
    Vector2 touch(mouseEvent->x, mouseEvent->y);    
    Vector3 direction = camera->screenToWorld(touch);
    
    if( field->getTouchPoint(camera->getPosition(), direction, point) )
    {
      PlayerCharacter *new_soldier = new PlayerSoldier("soldier", syukatsuGame, field);
      new_soldier->setPosition(position);
      new_soldier->setDestination(point);
      addChild(new_soldier);
    }
  }

  Actor::update(deltaTime);  
}
