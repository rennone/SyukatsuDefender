#include <syukatsu/GL/freeglut.h>
#include <sstream>
#include "Assets.h"
#include "Field.h"
#include "Character.h"
#include "PlayScene.h"
#include "TitleScene.h"
#include "SimpleObjectFactory.h"
#include "PlayerCharacterManager.h"
#include "EnemyCharacterManager.h"
#include "Barrack.h"
#include "Debugger.h"
using namespace std;

static void LightSetting()
{
  glEnable(GL_LIGHTING);    
  glEnable(GL_LIGHT0);
    
  GLfloat lightcol1[] = { 1.0, 0.7, 0.7, 1.0 };
  GLfloat lightpos1[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat lightdir1[] = { 1.0, -1.0, 1.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos1);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightdir1);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, lightcol1);  
}

PlayScene::PlayScene(SyukatsuGame *game)
  :SyukatsuScene(game)
{
  int width, height;
  glfwGetFramebufferSize(syukatsuGame->getWindow(), &width, &height);

  camera  = new MouseMoveCamera(syukatsuGame, 1, 5000, 45);
  menuCamera = new Camera2D(syukatsuGame->getWindow(), 48, 48);
  camera->setViewportWidth(width*3/4);
  camera->setViewportPosition(width*3/8, height/2);
  menuCamera->setViewportWidth(width/4);
  menuCamera->setViewportPosition(width*7/8, height/2);

  batcher = new SpriteBatcher(200);

  //全てのActorを一括してupdate, renderを行う為のルートアクター
  root = new Actor("root", syukatsuGame);

  field = new Field("field", syukatsuGame);
  
  //ルートアクターの子に追加
  root->addChild(field);

  //全てのプレイヤーを管理するクラス
  auto playerManager = new CharacterManager("aaa", syukatsuGame,field);
  auto enemyManager = new CharacterManager("bbb", syukatsuGame, field);

  auto barrack = new Barrack("barrack", syukatsuGame, field, playerManager);
  barrack->setPosition(Vector3(10, 0, 10));

  auto ebarrack = new Barrack("barrack2", syukatsuGame, field, enemyManager);
  ebarrack->setPosition(field->getPosition()+field->getSize()-Vector3(10,0,10));

  //全てのエネミーを管理するクラス
  root->addChild(playerManager);
  root->addChild(enemyManager);

  root->addChild(barrack);
  root->addChild(ebarrack);
  
  Assets::mincho->setSize(5);

  LightSetting();  
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

//デバッグ情報
  Debugger::drawDebugInfo("PlayScene.cpp", "FPS", 1.0/deltaTime);
  

  //characterのアップデートもまとめて行われる
  root->update(deltaTime);
}

void PlayScene::render(float deltaTime)
{
  camera->setViewportAndMatricesWithMouse();

  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
  root->render(deltaTime);  //全てのキャラクターの描画
  menuCamera->setViewportAndMatrices();
  glTranslatef(-12,0,0);

  Assets::mincho->render("this is Menu");

  glPopAttrib();

  Debugger::renderDebug(syukatsuGame->getWindow());
}

