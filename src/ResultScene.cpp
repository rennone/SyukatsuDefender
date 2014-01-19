#include "ResultScene.h"
#include "TitleScene.h"
#include "Assets.h"
ResultScene::ResultScene(SyukatsuGame *game, ResultScene::Result _result)
  :SyukatsuScene(game)
  ,result(_result)
{
  int width, height;
  glfwGetFramebufferSize(syukatsuGame->getWindow(), &width, &height);
  WIDTH = width;
  HEIGHT= height;
  
  camera = new Camera2D(syukatsuGame->getWindow(), WIDTH, HEIGHT);
  batcher = new SpriteBatcher(10);  
}

ResultScene::~ResultScene()
{
}
void ResultScene::update(float deltaTime)
{
  auto mouseEvent = syukatsuGame->getInput()->getMouseEvent();
  if(mouseEvent->action == GLFW_PRESS)
  {
    syukatsuGame->setScene(new TitleScene(syukatsuGame));
  }
}

void ResultScene::render(float deltaTime)
{
  camera->setViewportAndMatrices();
  batcher->beginBatch(Assets::textureAtlas);

  if(result == VICTORY)
    batcher->drawSprite(0, 0, WIDTH, HEIGHT, Assets::victory);
  else
    batcher->drawSprite(0, 0, WIDTH, HEIGHT, Assets::defeated);
  batcher->endBatch();
}

void ResultScene::dispose()
{
}

void ResultScene::reshape(int width, int height)
{
}



