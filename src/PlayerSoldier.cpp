#include "PlayerSoldier.h"
#include "Debugger.h"
#include "PlayScene.h"

PlayerSoldier::PlayerSoldier(string name, SyukatsuGame *game, Field *field)
  :Character(name, game, field)
{
  this->setAttributes(50, 10, 10, 50.0f, Vector3(1.0f, 0.0f, 0.0f)); 
}

void PlayerSoldier::update(float deltaTime)
{
  Vector2 p(position.x, position.z), d(destination.x, destination.z);
  
  if( p.distanceTo(d) < speed*deltaTime )
  {
    setStatus(Actor::Dead); //たどり着いたら死ぬ    
    ((PlayScene *)(syukatsuGame->getCurrentScene()))->siege();
    return;    
  }

  auto dir = d - p;  
  dir.normalize();

  Vector2 move = dir*deltaTime*speed;
  Vector2 after = p + move;
  Vector2 cPos, normal;
  
  auto list = field->enemyManager->getChildren();
  auto pList = field->playerManager->getChildren();
  list.insert(list.end(), pList.begin(), pList.end());
  
  for (auto child : list)
  {
    if(child->getStatus() != Actor::Action)
      continue;
    
    auto enemy_collider = child->getCollider();
    
    if( enemy_collider->collisionCheck(collider , p, after, cPos, normal) )
    {
      Debugger::drawDebugInfo("PlayerSoldier.cpp", "update", "true");
      after = cPos + normal*normal.dot(p-after)*1.1;
    }    
  }

  Vector3 after3 = Vector3(after.x, 0, after.y);
  
  field->collision(position, after3, radius); 
  position = after3;  

  Character::update(deltaTime);
}

