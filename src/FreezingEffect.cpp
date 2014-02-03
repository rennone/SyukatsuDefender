#include "FreezingEffect.h"
#include "Character.h"
#include <syukatsu/GL/glut.h>
#include <cmath>
FreezingEffect::FreezingEffect(string name, SyukatsuGame *game)
  :Effect(name, game)
  ,rot(0)
  ,target(NULL)
{
  setStatus(Actor::NoUse);
}

FreezingEffect::~FreezingEffect()
{
}

void FreezingEffect::render(float deltaTime)
{
  glPushMatrix();
  glTranslatef(position.x, position.y, position.z);
  glRotatef(rot, 0, 1, 0);
  glutSolidCube(30);
  glPopMatrix();
}

#include <iostream>
using namespace std;
void FreezingEffect::update(float deltaTime)
{
  const float speed = 100;
  rot += deltaTime * speed;
  if( target == NULL || target->getStatus() != Actor::Action || !target->isFrozen() )
  {
    setStatus(Actor::NoUse);
    target = NULL;
    return;
  }
  
  setPosition(target->getPosition());
}
