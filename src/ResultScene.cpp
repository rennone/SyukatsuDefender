#include "ResultScene.h"
#include "TitleScene.h"
#include "Assets.h"
#include "Information.h"
#include "MessageManager.h"
ResultScene::ResultScene(SyukatsuGame *game, Result _result, int _waveNum, float _time)
  :SyukatsuScene(game)
  ,result(_result)
  ,defenseTime(_time)
  ,waveNum(_waveNum)
  ,elapsedTime(0)
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
  elapsedTime += deltaTime;
  if(syukatsuGame->getInput()->isAnyKeyPressed())
  {
    syukatsuGame->setScene(new TitleScene(syukatsuGame));
  }
}

void ResultScene::render(float deltaTime)
{
  glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  camera->setViewportAndMatrices();

  //結果の描画
  glColor4f(1,1,1,1);
  batcher->beginBatch(Assets::resultAtlas);
  if(result == VICTORY)
    batcher->drawSprite(0, HEIGHT*0.2, 0.5*WIDTH, 0.2*HEIGHT, Assets::victory);
  else
    batcher->drawSprite(0, HEIGHT*0.2, 0.5*WIDTH, 0.2*HEIGHT, Assets::defeated);  
  batcher->endBatch();

  //成績の描画
  const float charSize = WIDTH/20;
  stringstream ss;
  ss << "wave " << waveNum;
  MessageManager::getInstance()->drawBitmapString(ss.str().c_str(),
                                                  Vector2(-WIDTH*0.4, -HEIGHT*0.1),
                                                  charSize);
  ss.str(""); // バッファをクリアする。
  ss.clear(stringstream::goodbit);// ストリームの状態をクリアする。この行がないと意図通りに動作しない
  ss << "time " << floor(defenseTime*10)/10;  
  MessageManager::getInstance()->drawBitmapString(ss.str().c_str(),
                                                  Vector2(-WIDTH*0.4, -HEIGHT*0.1-charSize),
                                                  charSize);
    //PressKeyの描画
  glColor4f(1,1,1,pow(sin(elapsedTime*2),2));
  batcher->beginBatch(Assets::resultAtlas);
  batcher->drawSprite(0, -HEIGHT*0.4, WIDTH/2, Assets::pressKey->getRatio() * WIDTH/2, Assets::pressKey);
  batcher->endBatch();

  glPopMatrix();
}

void ResultScene::dispose()
{
}

void ResultScene::reshape(int width, int height)
{
}



