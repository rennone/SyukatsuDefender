#include <syukatsu/GL/freeglut.h>
#include <sstream>
#include "Assets.h"
#include "Field.h"
#include "Character.h"
#include "PlayScene.h"
#include "TitleScene.h"
#include "SimpleObjectFactory.h"
#include "Barrack.h"
#include "LightningTower.h"
#include "TextBox.h"
#include "Debugger.h"
using namespace std;

static Vector3 debugPos;

static void LightSetting()
{
  //glEnable(GL_LIGHTING);    
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  glEnable(GL_LIGHT3);
  
  GLfloat lightcol1[] = { 1.0, 0.7, 0.7, 1.0 };
  GLfloat lightpos1[] = { 0.0, 500.0, 0.0, 1.0 };
  GLfloat lightdir1[] = { 1.0, -1.0, 1.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos1);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightdir1);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, lightcol1);
  
  GLfloat lightpos2[] = { 1000.0, 500.0, 1000.0, 1.0 };
  GLfloat lightdir2[] = { -1.0, -1.0, -1.0, 1.0 };
  glLightfv(GL_LIGHT1, GL_POSITION, lightpos2);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightdir2);
 
  GLfloat lightpos3[] = { 0.0, 500.0, 1000.0, 1.0 };
  GLfloat lightdir3[] = { 1.0, -1.0, -1.0, 1.0 };
  glLightfv(GL_LIGHT2, GL_POSITION, lightpos3);
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightdir3);
 
  GLfloat lightpos4[] = { 1000.0, 500.0, 0.0, 1.0 };
  GLfloat lightdir4[] = { -1.0, -1.0, 1.0, 1.0 };
  glLightfv(GL_LIGHT4, GL_POSITION, lightpos4);
  glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, lightdir4);
 
}

PlayScene::PlayScene(SyukatsuGame *game)
  :SyukatsuScene(game), menuPos(0)
{
  int width, height;
  glfwGetFramebufferSize(syukatsuGame->getWindow(), &width, &height);

  camera  = new MouseMoveCamera(syukatsuGame, 1, 3000, 45);
  
  menuCamera = new Camera2D(syukatsuGame->getWindow(), 48, 48);
  camera->setViewportWidth(width*3.0/4);
  camera->setViewportPosition(width*3.0/8, height/2);
  
  menuCamera->setViewportWidth(width/4.0);
  menuCamera->setViewportPosition(width*7.0/8, height/2.0);

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
  playerBuildingManager = new CharacterManager("bbb", syukatsuGame, field);

  enemyManager = new CharacterManager("bbb", syukatsuGame, field);
  enemyBuildingManager = new CharacterManager("ccc", syukatsuGame, field);
  
  playerManager->setTarget(enemyStronghold);
  enemyManager->setTarget(playerStronghold);

  playerManager->setColor(Vector3(1.0, 0.0, 0.0));
  enemyManager->setColor(Vector3(0.0, 1.0, 0.0));

  auto barrack = new Barrack("barrack", syukatsuGame, field, playerManager);
  auto ebarrack = new LightningTower("barrack2", syukatsuGame, field, playerManager);

  barrack->setPosition(playerStronghold);
  ebarrack->setPosition(enemyStronghold);

  playerBuildingManager->addChild(barrack);
  enemyBuildingManager->addChild(ebarrack);


  //全てのエネミーを管理するクラス
  root->addChild(playerManager);
  root->addChild(enemyManager);

  root->addChild(playerBuildingManager);
  root->addChild(enemyBuildingManager);
  
  Assets::mincho->setSize(3);

  LightSetting();  
}

PlayScene::~PlayScene()
{
  glDisable(GL_LIGHTING);    
  glDisable(GL_LIGHT0);
}


void PlayScene::update(float deltaTime)
{
  Vector3 point;
  auto mouseEvent = syukatsuGame->getInput()->getMouseEvent();  
  Vector2 touch(mouseEvent->x, mouseEvent->y);
  Vector3 direction = camera->screenToWorld(touch);
  field->updateMousePosition(camera->getPosition(), direction);
  

  auto keyEvents = syukatsuGame->getInput()->getKeyEvents();
  for(auto event : keyEvents)
  {
    if(event->action != GLFW_PRESS || event->keyCode != GLFW_KEY_ENTER)
      continue;

    syukatsuGame->setScene(new TitleScene(syukatsuGame));
    return;    
  }

  //メニュー
  if(syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_B)) {
    menuPos = (menuPos == 1 ? 0 : 1);
  }
  else if(syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_L)) {
    menuPos = (menuPos == 2 ? 0 : 2);
  }  

  //建設
  if(syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_C)) {
    if(menuPos == 0) {
    }
    else if(menuPos == 1) {
//      if(playerManager->getGold() >= 100 && field->getCollisionPoint(camera->getPosition(), direction, point)) {
      if(playerManager->getGold() >= 100 && field->getMouseCollisionPoint(point)) {
	auto testBarrack = new Barrack("barrack", syukatsuGame, field, playerManager);

        testBarrack->setPosition(point);	
	testBarrack->setPicked(true);

	playerBuildingManager->addChild(testBarrack);	
	//playerManager->subGold(100);
      }

      menuPos = 0;
    } 
  }

  //デバッグ情報
  Debugger::drawDebugInfo("PlayScene.cpp", "FPS", 1.0/deltaTime);
  Debugger::drawDebugInfo("PlayScene.cpp", "gold", playerManager->getGold());
  Debugger::drawDebugInfo("PlayScene.cpp", "menu", menuPos); 

  auto allyList = playerManager->getChildren();
  auto enemyList = enemyManager->getChildren();

  for(auto enemy : enemyList) {
    for(auto ally : allyList) {
      if( ((Character*)enemy)->isHit((Character*)ally)) {
	//敵を撃破
	if(((Character *)enemy)->gotDamage(1)) {
	  //playerManager->addGold(10);
	}

	((Character *)ally)->gotDamage(1);
      }
    }
  }
/*  
  if( field->getCollisionPoint(camera->getPosition(), direction, point) )
  {
    Debugger::drawDebugInfo("PlayScene.cpp", "fieldCollision", "true");    
    if(mouseEvent->action == GLFW_PRESS && syukatsuGame->getInput()->getKeyState(GLFW_KEY_R) == GLFW_REPEAT)
      debugPos = point;
  }
*/
  //characterのアップデートもまとめて行われる
  root->update(deltaTime);
  root->checkStatus();

  //cout << "update end" << endl;  
}

void PlayScene::render(float deltaTime)
{
  glEnable(GL_LIGHTING);
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

  static TextBox *textbox = new TextBox("This is Menu", Vector2(-24, -24), Vector2(48, 12), 5);
  static SpriteBatcher *batcher = new SpriteBatcher(10);
  
  glDisable(GL_LIGHTING);
  menuCamera->setViewportAndMatrices();

  batcher->beginBatch(Assets::textureAtlas);
  batcher->drawSprite(0,0,48,48, Assets::background);  
  textbox->render(false, batcher);
  batcher->endBatch();

  drawMenuString(1, "Barrack", Vector3(-12, 20, 0));
  drawMenuString(2, "LightningTower", Vector3(-12, 10, 0));

  Debugger::drawDebugInfo("PlayScene.cpp", "cameraSize", menuCamera->getFrustumSize());
  Debugger::drawDebugInfo("PlayScene.cpp", "cameraPos", menuCamera->getPosition());

  Assets::textureAtlas->unbind();  
  glPopAttrib();
  
  Debugger::renderDebug(syukatsuGame->getWindow());
}

void PlayScene::drawMenuString(int id, string name, const Vector3& pos)
{
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);

  glPushMatrix();
  glTranslatef(pos.x, pos.y, pos.z);
  if(menuPos == id) {
    glColor3d(1.0, 0.0, 0.0);
  }
  else {
    glColor3d(1.0, 1.0, 1.0);
  }
  
  Assets::mincho->render(name.c_str());

  glPopMatrix();

  glPopAttrib();
}
