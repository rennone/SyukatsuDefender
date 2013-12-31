#include "TitleScene.h"
#include "PlayScene.h"
#include "Assets.h"

static Vector2 target;

TitleScene::TitleScene(SyukatsuGame *game)
  :SyukatsuScene(game)
{
  camera = new Camera2D(syukatsuGame->getWindow(), WIDTH, HEIGHT);
  batcher = new SpriteBatcher(10);    
}

TitleScene::~TitleScene()
{
  delete camera;
  delete batcher;
}

void TitleScene::update(float deltaTime)
{
  auto keyEvents = game->getInput()->getKeyEvents();
  for(auto event : keyEvents)
  {
    if(event->action != GLFW_PRESS || event->keyCode != GLFW_KEY_ENTER)
      continue;

    syukatsuGame->setScene(new PlayScene(syukatsuGame));    
  }
  auto mouseEvent = game->getInput()->getMouseEvent();
  Vector2 touch(mouseEvent->x, mouseEvent->y);
  target = camera->screenToWorld(touch);
}

void TitleScene::render(float deltaTime)
{
  camera->setViewportAndMatrices();
  /*
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);
  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  
  glBegin(GL_TRIANGLES);
  glColor3d(1.0, 0.0, 0.0);  
  glVertex3f(-WIDTH/2, -HEIGHT/2, 0.f); 
  glVertex3f(-WIDTH/2, +HEIGHT/2, 0.f);
  glVertex3f(+WIDTH/2, +HEIGHT/2, 0.f);
  glEnd();

  //glColor3d(1.0, 1.0, 0.0);  

  Assets::textureAtlas->bind();  
  glBegin(GL_TRIANGLES);
  glTexCoord2f(Assets::background->u1, Assets::background->v1);  glVertex3f(-WIDTH/2, -HEIGHT/2, 0.f); 
  glTexCoord2f(Assets::background->u2, Assets::background->v1);  glVertex3f(-WIDTH/2, +HEIGHT/2, 0.f);
  glTexCoord2f(Assets::background->u2, Assets::background->v2);  glVertex3f(+WIDTH/2, +HEIGHT/2, 0.f);
  glEnd();
  */
/*
  batcher->beginBatch(Assets::textureAtlas);
  batcher->drawSprite(-0.02*WIDTH, 0, WIDTH*1, HEIGHT*1, Assets::background);  
  //batcher->drawSprite(0, 0, WIDTH/2, HEIGHT/4, Assets::start);
  //batcher->drawSprite(target.x, target.y, HEIGHT/20, HEIGHT/20, Assets::bullet);  
  batcher->endBatch();
*/
}

void TitleScene::reshape(int width, int height)
{
  
}

void TitleScene::dispose()
{
  
}
