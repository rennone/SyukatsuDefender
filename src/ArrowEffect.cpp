#include "ArrowEffect.h"
#include "Character.h"
#include <syukatsu/GL/glut.h>

ArrowEffect::ArrowEffect(string name, SyukatsuGame *game)
  :Effect(name, game)
  ,target(NULL)
  ,attack(0)
  ,speed(0)
{
  setStatus(Actor::NoUse);
}

ArrowEffect::~ArrowEffect()
{
}

#include <iostream>
using namespace std;
void ArrowEffect::update(float deltaTime)
{
  if ( target == NULL || target->getStatus() != Actor::Action )
  {
    setStatus(Actor::NoUse);
    return;
  }
 
  if ( position.distSquared(target->getPosition()) <pow( deltaTime*speed, 2) )
  {
    target->gotDamage(attack);
    setStatus(Actor::NoUse);
  }  

  auto direction = target->getPosition() - position;
  direction.normalize();

  position += speed*deltaTime*direction;
  Actor::update(deltaTime);
}

void ArrowEffect::render(float deltaTime)
{
  glPushMatrix();
  glTranslatef(position.x, position.y, position.z);
  glutSolidSphere(5, 5, 5);
  glPopMatrix();
  Actor::render(deltaTime);
}
