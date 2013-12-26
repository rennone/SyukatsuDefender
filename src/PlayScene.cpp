
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
using namespace std;

PlayScene::PlayScene(SyukatsuGame *game)
  :SyukatsuScene(game)
{
  camera  = new MouseMoveCamera(syukatsuGame, 1, 1000, 45);
  batcher = new SpriteBatcher(200);

  //全てのActorを一括してupdate, renderを行う為のルートアクター
  root = new Actor("root", syukatsuGame);

  //キャラクターの生成
  character = new Character("sampleCharacter", syukatsuGame);
  character->setPosition(30, 50, 20);

  field = new Field("field", syukatsuGame);
  
  //ルートアクターの子に追加
  root->addChild(character);
  root->addChild(field);

  //全てのプレイヤーを管理するクラス
root->addChild(new PlayerCharacterManager("playerCharacterManager", syukatsuGame, camera, field));

  //全てのエネミーを管理するクラス
  root->addChild(new EnemyCharacterManager("enemyCharacterManager", syukatsuGame));                 
  
  Assets::mincho->setSize(3);  
}

#include <iostream>
void PlayScene::update(float deltaTime)
{
  auto keyEvents = syukatsuGame->getInput()->getKeyEvents();
  for(auto event : keyEvents)
  {
    if(event->action != GLFW_PRESS || event->keyCode != GLFW_KEY_ENTER)
      continue;

    syukatsuGame->setScene(new TitleScene(syukatsuGame));    
  }
  
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
  
  glPopAttrib();

  /*
  batcher->beginBatch(Assets::textureAtlas);
  root->render(deltaTime);
  batcher->endBatch();
  glTranslatef(-30,0,0);  
  Assets::mincho->render("enter : back to title");
  */
}

