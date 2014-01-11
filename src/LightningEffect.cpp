#include "LightningEffect.h"

#include "SimpleObjectFactory.h"

LightningEffect::LightningEffect(string name, SyukatsuGame *game, Vector3 _position)
  :Effect(name, game)
  ,position(_position)
{
  
}

LightningEffect::~LightningEffect()
{
  
}


LightningEffect::update(float deltaTime)
{
  
}

LightningEffect::render(float deltaTime)
{
  Assets::textureAtlas->bind();  
  drawTexture(position, Vector3(0,1,0), 20, Assets::virus);  
}



