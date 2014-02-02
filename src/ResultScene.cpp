#include "ResultScene.h"
#include "TitleScene.h"
#include "Assets.h"
#include "Information.h"
#include "MessageManager.h"
ResultScene::ResultScene(SyukatsuGame *game, Result _result, int _waveNum, float _elapsedTime)
  :SyukatsuScene(game)
  ,result(_result)
  ,waveNum(_waveNum)
  ,elapsedTime(_elapsedTime)
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

static void setting2D()
{
  
}
void ResultScene::render(float deltaTime)
{
  glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  camera->setViewportAndMatrices();
  
  batcher->beginBatch(Assets::resultAtlas);
  if(result == VICTORY)
    batcher->drawSprite(0, HEIGHT*0.4, 0.5*WIDTH, 0.2*HEIGHT, Assets::victory);
  else
    batcher->drawSprite(0, HEIGHT*0.4, 0.5*WIDTH, 0.2*HEIGHT, Assets::defeated);
  
  batcher->drawSprite(0, -HEIGHT*0.4, 0.5*WIDTH, 0.2*HEIGHT, Assets::pressKey);
  batcher->endBatch();

  stringstream ss;
  ss << "wave " << waveNum;
  MessageManager::drawMessage(ss.str().c_str(), Vector2(-WIDTH*0.4, HEIGHT*0.1), 1, YellowText);

  ss.str(""); // バッファをクリアする。
  ss.clear(stringstream::goodbit);// ストリームの状態をクリアする。この行がないと意図通りに動作しない
  ss << "time " << elapsedTime;
  
  MessageManager::drawMessage(ss.str().c_str(), Vector2(-WIDTH*0.4, -HEIGHT*0.1),1, YellowText);
  MessageManager::render2DMessage(deltaTime);

  glPopMatrix();
}

void ResultScene::dispose()
{
}

void ResultScene::reshape(int width, int height)
{
}



