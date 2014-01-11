#include "LightningEffect.h"
#include "SimpleObjectFactory.h"
#include "Assets.h"

LightningEffect::LightningEffect(string name, SyukatsuGame *game)
  :Effect(name, game)
  ,elapsedTime(0)
{
  range = 100;
  setStatus(Actor::NoUse);  
}

LightningEffect::~LightningEffect()
{
  
}


void LightningEffect::update(float deltaTime)
{
  elapsedTime += deltaTime;
  if( (elapsedTime+=deltaTime) > time)
    setStatus(Actor::NoUse);  
}

void LightningEffect::render(float deltaTime)
{
  Assets::textureAtlas->bind();
  drawTexture(position+Vector3(0,20, 0), Vector3(0,1,0), range*sin(elapsedTime), Assets::start);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void LightningEffect::playEffect(Vector3 position, float time)
{
  this->position = position;
  this->time = time;
  elapsedTime = 0;  
  setStatus(Actor::Action);  
}




