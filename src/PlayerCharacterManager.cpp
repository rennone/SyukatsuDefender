#include "PlayerCharacterManager.h"
#include "PlayerSoldier.h"
#include "PlayerBowman.h"
#include "Debugger.h"
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
    cout << "S" << endl;
    
    PlayerCharacter *new_bowman = new PlayerBowman("bowman", syukatsuGame, field);
    new_bowman->setPosition(position);
    addChild(new_bowman);    
  }

  Actor::update(deltaTime);  
}
