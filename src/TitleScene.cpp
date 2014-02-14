#include "TitleScene.h"
#include "StageSelectScene.h"
#include "Assets.h"
#include "Debugger.h"

TitleScene::TitleScene(SyukatsuGame *game)
  :SyukatsuScene(game)
  ,elapsedTime(0)
{
  camera = new Camera2D(syukatsuGame->getWindow(), WIDTH, HEIGHT);
  batcher = new SpriteBatcher(110);
}

TitleScene::~TitleScene()
{
  delete camera;
  delete batcher;
}

void TitleScene::update(float deltaTime)
{
  elapsedTime += deltaTime;
  auto keyEvents = game->getInput()->getKeyEvents();

  if(game->getInput()->isAnyKeyPressed())
  {
    syukatsuGame->setScene(new StageSelectScene(syukatsuGame));    
  }
}

void TitleScene::render(float deltaTime)
{
  glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);
  camera->setViewportAndMatrices();
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glColor4f(1,1,1,1);

  batcher->beginBatch(Assets::titleAtlas);
//  batcher->drawSprite(0, 0, WIDTH*1, HEIGHT*1, Assets::background);
  batcher->drawSprite(0, HEIGHT/5, 4, Assets::titleLogo->getRatio() * 4, Assets::titleLogo);
  batcher->endBatch(); 
  
  batcher->beginBatch(Assets::titleAtlas);
  batcher->drawSprite(0, -HEIGHT/4, 3, Assets::pressKey->getRatio() * 3, Assets::pressKey);
  glColor4f(1,1,1,pow(sin(elapsedTime*2),2));
  batcher->endBatch();
  
  Debugger::renderDebug(syukatsuGame->getWindow());
  glPopAttrib();
}

void TitleScene::reshape(int width, int height)
{
  
}

void TitleScene::dispose()
{
  
}
