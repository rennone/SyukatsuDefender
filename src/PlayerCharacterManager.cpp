#include "PlayerCharacterManager.h"
#include "PlayerSoldier.h"
#include "PlayerBowman.h"
#include "Debugger.h"
#include <iostream>
using namespace std;

void PlayerCharacterManager::update(float deltaTime)
{
  auto mouseEvent = syukatsuGame->getInput()->getMouseEvent();

  Debugger::drawDebugInfo("PlayCharacterManager.cpp", "viewPosition", camera->getViewportPosition());
  Debugger::drawDebugInfo("PlayCharacterManager.cpp", "viewWidth", camera->getViewportWidth());
  Debugger::drawDebugInfo("PlayCharacterManager.cpp", "viewHeight", camera->getViewportHeight());
  Vector3 point;
  if(mouseEvent->action == GLFW_PRESS)
  {
    Vector2 touch(mouseEvent->x, mouseEvent->y);    
    Vector3 direction = camera->screenToWorld(touch);
    
    if( field->getCollisionPoint(camera->getPosition(), direction, point) )
    {
      PlayerCharacter *new_soldier = new PlayerSoldier("soldier", syukatsuGame, field);
      new_soldier->setPosition(position);
      new_soldier->setDestination(point);
      addChild(new_soldier);
    }
  }

  if(syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_A))
  {
    Vector2 touch(mouseEvent->x, mouseEvent->y);    
    Vector3 direction = camera->screenToWorld(touch);    
    PlayerCharacter *new_bowman = new PlayerBowman("bowman", syukatsuGame, field);
    new_bowman->setPosition(camera->getPosition());
    new_bowman->setDestination(camera->getPosition() + 3000*direction);    
    addChild(new_bowman);    
  }

  Actor::update(deltaTime);  
}
