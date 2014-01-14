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
#include "FreezingTower.h"
#include "TextBox.h"
#include "Debugger.h"
#include "TestCharacter.h"
#include "IconList.h"

using namespace std;

static Vector3 debugPos;
static IconList *debugIconList;

float PlayScene::MENU_WINDOW_WIDTH;
float PlayScene::MENU_WINDOW_HEIGHT;

float PlayScene::getMenuWindowWidth()
{
  return MENU_WINDOW_WIDTH;
}
float PlayScene::getMenuWindowHeight()
{
  return MENU_WINDOW_HEIGHT;
}


static void LightSetting()
{
  glEnable(GL_LIGHTING);    
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
  :SyukatsuScene(game), menuPos(0), health(1), nowWave(1), buildMode(true)
{
  int width, height;
  glfwGetFramebufferSize(syukatsuGame->getWindow(), &width, &height);

  const int playWidth  = width*3.0/4.0;
  const int playHeight = height;

  const int menuWidth  = width-playWidth;
  const int menuHeight = height;
  const float menuRatio  = menuWidth/(float)menuHeight;

  MENU_WINDOW_HEIGHT = 100.0f;
  MENU_WINDOW_WIDTH  = MENU_WINDOW_HEIGHT*menuRatio;
  
  //メニュー画面のカメラ
  menuCamera = new Camera2D(syukatsuGame->getWindow(), MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
  menuCamera->setViewportWidth(menuWidth);
  menuCamera->setViewportHeight(menuHeight);  
  menuCamera->setViewportPosition(playWidth+menuWidth/2, menuHeight/2.0);

  //プレイ画面のカメラ
  camera  = new MouseMoveCamera(syukatsuGame, 1, 3000, 60);  
  camera->setViewportWidth(playWidth);
  camera->setViewportHeight(playHeight);
  camera->setViewportPosition(playWidth/2, playHeight/2);
  
  batcher = new SpriteBatcher(200);
  //全てのActorを一括してupdate, renderを行う為のルートアクター
  root = new Actor("root", syukatsuGame);

  //フィールドの生成
  field = new Field("field", syukatsuGame, NULL, NULL);
  
  //ルートアクターの子に追加
  root->addChild(field);
  
  const Vector3 playerStronghold = Vector3(10, 0, 10);
  const Vector3 enemyStronghold = Vector3(field->getPosition()+field->getSize()-Vector3(10,0,10));

  //全てのプレイヤーを管理するクラス
  playerManager = new CharacterManager("aaa", syukatsuGame, field);
  playerBuildingManager = new CharacterManager("bbb", syukatsuGame, field);

  enemyManager = new CharacterManager("bbb", syukatsuGame, field);
  enemyBuildingManager = new CharacterManager("ccc", syukatsuGame, field);

  field->playerManager = playerManager;
  field->enemyManager  = enemyManager;
  
  playerManager->setTarget(enemyStronghold - Vector3(10, 0, 0));
  enemyManager->setTarget(playerStronghold + Vector3(10, 0, 0));

  playerManager->setColor(Vector3(1.0, 0.0, 0.0));
  enemyManager->setColor(Vector3(0.0, 1.0, 0.0));

  //全てのエネミーを管理するクラス
  root->addChild(playerManager);
  root->addChild(enemyManager);

  root->addChild(playerBuildingManager);
  root->addChild(enemyBuildingManager);
  
  Assets::mincho->setSize(5);
  
  LightSetting();
  debugIconList = new IconList("iconList", syukatsuGame);
  
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

  Vector2 cell;
  if(mouseEvent->action == GLFW_PRESS)
  {
    if(debugIconList->getSelectIcon() != -1 && playerManager->getGold() >= 100 && field->getMouseCollisionCell(cell))
    {
      auto testBarrack = new LightningTower("barrack", syukatsuGame, field, enemyManager);
//      field->setBuildingInField(cell, 1);
      field->setBuilding(testBarrack, cell.x, cell.y);
      testBarrack->setPosition(field->cellToPoint(cell.x, cell.y));
      testBarrack->setPicked(true);

      playerBuildingManager->addChild(testBarrack);
      //playerManager->subGold(100);
    }    
    debugIconList->selectIcon(menuCamera->screenToWorld(touch));
  }

  field->updateMousePosition(camera->getPosition(), direction);
  
  if(buildMode) {
    if(syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_S)) {
      startWave(1);
      buildMode = false;
    }

  }
  else {
    //ゲーム終了
    //敗北
    if(health <= 0) { 
      syukatsuGame->setScene(new TitleScene(syukatsuGame));
    }

    //勝利
    if(remainEnemy <= 0) {
      syukatsuGame->setScene(new TitleScene(syukatsuGame));
    }

    auto keyEvents = syukatsuGame->getInput()->getKeyEvents();
    for(auto event : keyEvents)
    {
	if(event->action != GLFW_PRESS || event->keyCode != GLFW_KEY_ENTER)
	  continue;

	syukatsuGame->setScene(new TitleScene(syukatsuGame));
	return;    
    }
  }

  //メニュー
  if(syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_L)) {
    menuPos = (menuPos == 1 ? 0 : 1);
  }
  else if(syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_F)) {
    menuPos = (menuPos == 2 ? 0 : 2);
  }

  //建設
  if(syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_C) ||
     (mouseEvent->action == GLFW_PRESS && mouseEvent->button == GLFW_MOUSE_BUTTON_LEFT) ) {
    if(menuPos == 0) {
    }
    else if(menuPos == 1) {
      if(playerManager->getGold() >= 100 && field->getMouseCollisionCell(cell))
      {
	auto testBarrack = new LightningTower("barrack", syukatsuGame, field, enemyManager);        
        field->setBuildingInField(cell, 1);
        testBarrack->setPosition(field->cellToPoint(cell.x, cell.y));        
	testBarrack->setPicked(false);        
        
	playerBuildingManager->addChild(testBarrack);	
	//playerManager->subGold(100);
      }
      menuPos = 0;
    } 
    else if(menuPos == 2) {
      if(playerManager->getGold() >= 100 && field->getMouseCollisionCell(cell))
      {

	auto testBarrack = new FreezingTower("barrack", syukatsuGame, field, enemyManager);        
        field->setBuildingInField(cell, 1);
        testBarrack->setPosition(field->cellToPoint(cell.x, cell.y));        
	testBarrack->setPicked(false);        
        
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
  Debugger::drawDebugInfo("PlayScene.cpp", "enemy", remainEnemy);

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
  //characterのアップデートもまとめて行われる
  root->update(deltaTime);
  root->checkStatus();
}

void PlayScene::render(float deltaTime)
{
  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);

  glEnable(GL_LIGHTING);  
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  glEnable(GL_LIGHT3);
    
  camera->setViewportAndMatricesWithMouse();
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  root->render(deltaTime);  //全てのキャラクターの描画

  glPopAttrib();

  
  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
  glDisable(GL_DEPTH_TEST);  //これがあると2Dでは, 透過画像が使えないので消す
  glDisable(GL_LIGHTING);  
  static TextBox *textbox = new TextBox("This is Menu",
                                        Vector2(-MENU_WINDOW_WIDTH/2, -MENU_WINDOW_HEIGHT/2),
                                        Vector2(MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT/5), 5);
  static SpriteBatcher *batcher = new SpriteBatcher(10);

  menuCamera->setViewportAndMatrices();  
  batcher->beginBatch(Assets::textureAtlas);
  batcher->drawSprite(0,0,MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT, Assets::background);  
  textbox->render(false, batcher);
  batcher->endBatch();

  debugIconList->render(deltaTime);
  
  drawMenuString(1, "Barrack", Vector3(-12, 20, 0));
  drawMenuString(2, "LightningTower", Vector3(-12, -10, 0));  
  Debugger::drawDebugInfo("PlayScene.cpp", "cameraSize", menuCamera->getFrustumSize());
  Debugger::drawDebugInfo("PlayScene.cpp", "cameraPos", menuCamera->getPosition());
  Assets::textureAtlas->unbind();
  glPopAttrib();
  
  Debugger::renderDebug(syukatsuGame->getWindow());
}

void PlayScene::drawMenuString(int id, string name, const Vector3& pos)
{
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_COLOR_MATERIAL);

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

void PlayScene::startWave(int waveNum) 
{
  remainEnemy = 10;
  nowWave = waveNum;

  const Vector3 playerStronghold = Vector3(10, 0, 10);
  const Vector3 enemyStronghold = Vector3(field->getPosition()+field->getSize()-Vector3(10,0,10));

  auto ebarrack = new Barrack("barrack2", syukatsuGame, field, enemyManager);
  ebarrack->setPosition(enemyStronghold);

  enemyBuildingManager->addChild(ebarrack);
}
