#include "ResultScene.h"
#include "TitleScene.h"
#include "Assets.h"
#include "Information.h"
#include "MessageManager.h"
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
  delete camera;
  delete batcher;
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
    batcher->drawSprite(0, HEIGHT*0.4, 0.5*WIDTH, 0.2*HEIGHT, Assets::victory);
  else
    batcher->drawSprite(0, HEIGHT*0.4, 0.5*WIDTH, 0.2*HEIGHT, Assets::defeated);

  
  batcher->drawSprite(0, -HEIGHT*0.4, 0.5*WIDTH, 0.2*HEIGHT, Assets::backHome);
  batcher->endBatch();
  MessageManager::drawMessage("wave", Vector2(-WIDTH*0.4, HEIGHT*0.1), 1, YellowText);
  MessageManager::drawMessage("time", Vector2(-WIDTH*0.4, -HEIGHT*0.1),1, YellowText);
  MessageManager::render2DMessage(deltaTime);
}

void ResultScene::dispose()
{
}

void ResultScene::reshape(int width, int height)
{
}



