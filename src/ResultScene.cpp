#include "ResultScene.h"

ResultScene::ResultScene(SyukatsuGame *game)
  :SyukatsuScene(game)
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
}

void ResultScene::render(float deltaTime)
{
}

void ResultScene::dispose()
{
}

void ResultScene::reshape(int width, int height)
{
}



