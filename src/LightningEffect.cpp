#include "LightningEffect.h"
#include "SimpleObjectFactory.h"
#include "Assets.h"
#include "PlayScene.h"
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

static int i = 0;
void LightningEffect::update(float deltaTime)
{
  elapsedTime += deltaTime;
  if( (elapsedTime+=deltaTime) > time)
    setStatus(Actor::NoUse);

  i = (int)(elapsedTime/time*20)%10;
}


void LightningEffect::render(float deltaTime)
{
  auto scene = (PlayScene*)(syukatsuGame->getCurrentScene());
  auto normal = scene->getCameraPosition() - position;
  normal.y = 0;
  normal.normalize();
  glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);
  glDisable(GL_LIGHTING);
  Assets::playAtlas->bind();  
  drawTexture(position+Vector3(0,20+60*(1-i/10.0), 0),
              normal,
              Vector2(30.0, 120.0*i/10.0), Assets::thunder[i]);
  glBindTexture(GL_TEXTURE_2D, 0);
  glPopAttrib();
}

void LightningEffect::playEffect(Vector3 position, float time)
{
  this->position = position;
  this->time = time;
  elapsedTime = 0;  
  setStatus(Actor::Action);  
}
