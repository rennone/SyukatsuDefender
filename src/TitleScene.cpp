#include "TitleScene.h"
#include "PlayScene.h"
#include "Assets.h"

void TitleScene::update(float deltaTime)
{
  auto keyEvents = game->getInput()->getKeyEvents();
  for(auto event : keyEvents)
  {
    if(event->action != GLFW_PRESS || event->keyCode != GLFW_KEY_ENTER)
      continue;

    syukatsuGame->setScene(new PlayScene(syukatsuGame));    
  }  
}

void TitleScene::render(float deltaTime)
{
  camera->setViewportAndMatrices();

  batcher->beginBatch(Assets::textureAtlas);
  batcher->drawSprite(0, 0, WIDTH/2, HEIGHT/4, Assets::start);
  batcher->endBatch();  
}

void TitleScene::reshape(int width, int height)
{
  
}

void TitleScene::dispose()
{
  
}
