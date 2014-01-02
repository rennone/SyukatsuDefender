#ifndef TEST_CHARACTER_H
#define TEST_CHARACTER_H

#include "Character.h"

class TestObject:public Character
{  
public:
  TestObject(string name, SyukatsuGame *game, Field *field)
    :Character(name,game,field)
  {
    radius = 30;
    collider->radius = radius;    
    position.set(500, 0, 550);    
  }      
};


#include <iostream>
using namespace std;

class TestCharacter:public Character
{
  Character *object;
  
public:
  TestCharacter(string name, SyukatsuGame *game, Field *field)
    :Character(name, game, field)
  {
    radius = 20;
    collider->radius = radius;
    
    object = new TestObject("object", game, NULL);    
  }

  void update(float deltaTime)
  {
    Vector2 p(position.x, position.z);
    Vector2 dir(1, 1);
    dir.normalize();
    Vector2 move = dir*deltaTime*60;
    Vector2 after = p + move;
    Vector2 cPos, normal;

    auto enemyCollider = object->getCollider();
    
    if(enemyCollider->collisionCheck(collider, p, after, cPos, normal))
    {
      after = cPos + normal*normal.dot(p-after);        
    }
    
    position.x = after.x;
    position.z = after.y;
    
  }
  
  void render(float deltaTime)
  {
    Character::render(deltaTime);
    object->render(deltaTime);    
  }
  
    
};


#endif
