#include <syukatsu/GL/freeglut.h>
#include <sstream>
#include "Assets.h"
#include "Field.h"
#include "Character.h"
#include "PlayScene.h"
#include "TitleScene.h"
#include "SimpleObjectFactory.h"
#include "Barrack.h"
#include "Debugger.h"
using namespace std;

static Vector3 debugPos;

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
  
  const Vector3 playerStronghold = Vector3(10, 0, 10);
  const Vector3 enemyStronghold = Vector3(field->getPosition()+field->getSize()-Vector3(10,0,10));

  //全てのプレイヤーを管理するクラス
  playerManager = new CharacterManager("aaa", syukatsuGame, field);
  enemyManager = new CharacterManager("bbb", syukatsuGame, field);
  
  playerManager->setTarget(enemyStronghold);
  enemyManager->setTarget(playerStronghold);

  playerManager->setColor(Vector3(1.0, 0.0, 0.0));
  enemyManager->setColor(Vector3(0.0, 1.0, 0.0));

  auto barrack = new Barrack("barrack", syukatsuGame, field, playerManager);
  auto ebarrack = new Barrack("barrack2", syukatsuGame, field, enemyManager);

  barrack->setPosition(playerStronghold);
  ebarrack->setPosition(enemyStronghold);

  //全てのエネミーを管理するクラス
  root->addChild(playerManager);
  root->addChild(enemyManager);

  root->addChild(barrack);
  root->addChild(ebarrack);
  
  Assets::mincho->setSize(5);

  LightSetting();  
}

PlayScene::~PlayScene()
{
  glDisable(GL_LIGHTING);    
  glDisable(GL_LIGHT0);

}


void PlayScene::update(float deltaTime)
{
  auto keyEvents = syukatsuGame->getInput()->getKeyEvents();
  for(auto event : keyEvents)
  {
    if(event->action != GLFW_PRESS || event->keyCode != GLFW_KEY_ENTER)
      continue;

    syukatsuGame->setScene(new TitleScene(syukatsuGame));
    return;    
  }

  //デバッグ情報
  Debugger::drawDebugInfo("PlayScene.cpp", "FPS", 1.0/deltaTime);

  Debugger::drawDebugInfo("PlayScene.cpp", "gold", playerManager->getGold());

  auto allyList = playerManager->getChildren();
  auto enemyList = enemyManager->getChildren();

  for(auto enemy : enemyList) {
    for(auto ally : allyList) {
      if( ((Character*)enemy)->isHit((Character*)ally)) {
	//敵を撃破
	if(((Character *)enemy)->gotDamage(1)) {
	  playerManager->addGold(10);
	}

	((Character *)ally)->gotDamage(1);
      }
    }
  }

  auto mouseEvent = syukatsuGame->getInput()->getMouseEvent();

  Vector3 point;
  Vector2 touch(mouseEvent->x, mouseEvent->y);
  Vector3 direction = camera->screenToWorld(touch);
  
  if( field->getCollisionPoint(camera->getPosition(), direction, point) )
  {
    Debugger::drawDebugInfo("PlayScene.cpp", "fieldCollision", "true");    
    if(mouseEvent->action == GLFW_PRESS && syukatsuGame->getInput()->getKeyState(GLFW_KEY_R) == GLFW_REPEAT)
      debugPos = point;
  }
  //characterのアップデートもまとめて行われる
  root->update(deltaTime);
  root->checkStatus();

  //cout << "update end" << endl;  
}

void PlayScene::render(float deltaTime)
{
  camera->setViewportAndMatricesWithMouse();

  static float elaspedTime = 0;
  elaspedTime += deltaTime;  
  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
    glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST); //アルファテスト開始  
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);

  Assets::textureAtlas->bind();  
  root->render(deltaTime);  //全てのキャラクターの描画
  glPushMatrix();
  glTranslatef(debugPos.x, debugPos.y + 30*sin(elaspedTime)*sin(elaspedTime), debugPos.z);
  glutSolidCube(10);  
  glPopMatrix();
  menuCamera->setViewportAndMatrices();
  glTranslatef(-12,0,0);
  Assets::mincho->render("this is Menu");

  Assets::textureAtlas->unbind();  
  glPopAttrib();
  
  Debugger::renderDebug(syukatsuGame->getWindow());
}

