#include "EnemyCharacterManager.h"
#include "EnemySoldier.h"

#include <iostream>
using namespace std;

void EnemyCharacterManager::update(float deltaTime)
{
  auto mouseEvent = syukatsuGame->getInput()->getMouseEvent();
  auto keyEvents = syukatsuGame->getInput()->getKeyEvents();
  
  Vector3 point;
  for(auto keyEvent : keyEvents) {
    if(keyEvent->action == GLFW_PRESS && keyEvent->keyCode == GLFW_KEY_X) {
	Vector2 touch(mouseEvent->x, mouseEvent->y);    
	Vector3 direction = camera->screenToWorld(touch);
    
	if( field->getTouchPoint(camera->getPosition(), direction, point) ) {
	  auto *new_soldier = new EnemySoldier("soldier", syukatsuGame, field);
 
	  new_soldier->setPosition(position);
	  new_soldier->setDestination(point);
	  addChild(new_soldier);
	}
    }
  }

  Actor::update(deltaTime);  
}
