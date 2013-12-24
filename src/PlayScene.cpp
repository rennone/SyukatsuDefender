//#include "Player.h"
#include "PlayScene.h"
#include "TitleScene.h"
#include "Assets.h"
//#include "Field.h"

#include <sstream>
using namespace std;

PlayScene::PlayScene(SyukatsuGame *game)
  :SyukatsuScene(game)
{
  camera  = new Camera2D(syukatsuGame->getWindow(), 60, 40);
  batcher = new SpriteBatcher(200);
  root = new Actor("root", syukatsuGame);
//  root->addChild(new Player("player", syukatsuGame, batcher));
  Assets::mincho->setSize(3);  
}

void PlayScene::update(float deltaTime)
{
  auto keyEvents = syukatsuGame->getInput()->getKeyEvents();
  for(auto event : keyEvents)
  {
    if(event->action != GLFW_PRESS || event->keyCode != GLFW_KEY_ENTER)
      continue;

    syukatsuGame->setScene(new TitleScene(syukatsuGame));    
  }
  
  root->update(deltaTime);
}

void PlayScene::render(float deltaTime)
{
  camera->setViewportAndMatrices();
  batcher->beginBatch(Assets::textureAtlas);
  root->render(deltaTime);
  batcher->endBatch();
  glTranslatef(-30,0,0);  
  Assets::mincho->render("enter : back to title");  
}

