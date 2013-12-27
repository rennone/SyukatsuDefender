
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
#include "Debugger.h"
using namespace std;

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
auto playerCharacterManager = new PlayerCharacterManager("playerCharacterManager", syukatsuGame, camera, field);
//playerCharacterManager->setPosition(field->getPosition()-field->getSize()/2);
playerCharacterManager->setPosition(Vector3(0,0,0));
  root->addChild(playerCharacterManager);

auto enemyCharacterManager = new EnemyCharacterManager("enemyCharacterManager", syukatsuGame, camera, field);
enemyCharacterManager->setPosition(field->getPosition()+field->getSize()/2);

  //全てのエネミーを管理するクラス
  root->addChild(enemyCharacterManager );
  
  Assets::mincho->setSize(5);  
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
Debugger::drawDebugInfo("PlayScene.cpp", "tag", "value");

  //characterのアップデートもまとめて行われる
  root->update(deltaTime);
}

void PlayScene::render(float deltaTime)
{
  camera->setViewportAndMatricesWithMouse();

  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
  //glEnable(GL_LIGHTING);    
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  glEnable(GL_LIGHT3);
  glEnable(GL_DEPTH_TEST);
    
  GLfloat color[]     = {    1.0,    1.0,    1.0, 1.0 };
  GLfloat lightpos1[] = {  200.0, -200.0, -200.0, 1.0 };
  GLfloat lightpos2[] = { -200.0,  200.0,  200.0, 1.0 };
  GLfloat lightpos3[] = { -200.0, -200.0, -200.0, 1.0 };
  GLfloat lightpos4[] = {  200.0,  200.0,  200.0, 1.0 };
    
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos1);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
  glLightfv(GL_LIGHT1, GL_POSITION, lightpos2);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, color);
    
  glLightfv(GL_LIGHT2, GL_POSITION, lightpos3);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, color);
  glLightfv(GL_LIGHT3, GL_POSITION, lightpos4);
  glLightfv(GL_LIGHT3, GL_DIFFUSE, color);
  root->render(deltaTime);  //全てのキャラクターの描画
  menuCamera->setViewportAndMatrices();
  glTranslatef(-12,0,0);

  Assets::mincho->render("this is Menu");

  glPopAttrib();

  Debugger::renderDebug(syukatsuGame->getWindow());
}

