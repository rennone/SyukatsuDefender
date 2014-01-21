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
#include "Information.h"
#include "Message.h"
#include "MessageManager.h"
#include "ResultScene.h"
#include "Soldier.h"
#include <sstream>

using namespace std;

float PlayScene::MENU_WINDOW_WIDTH;
float PlayScene::MENU_WINDOW_HEIGHT;


Character *debug_character;

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
  :SyukatsuScene(game), health(1), nowWave(1), buildMode(true)
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

  //プレイ画面のカメラ(2D)
  playCamera2D = new Camera2D(syukatsuGame->getWindow(), playWidth, playHeight);
  playCamera2D->setViewportWidth(playWidth);
  playCamera2D->setViewportHeight(playHeight);
  playCamera2D->setViewportPosition(playWidth/2, playHeight/2);
  
  //全てのActorを一括してupdate, renderを行う為のルートアクター
  root = new Actor("root", syukatsuGame);

  //フィールドの生成
  field = new Field("field", syukatsuGame, NULL, NULL);
  
  //ルートアクターの子に追加
  root->addChild(field);
  
  const Vector3 playerStronghold = Vector3(10, 0, 10);
  const Vector3 enemyStronghold = Vector3(field->getPosition()+field->getSize()-Vector3(10,0,10));

  //全てのプレイヤーを管理するクラス
  playerManager         = new CharacterManager("aaa", syukatsuGame, field);
  playerBuildingManager = new CharacterManager("bbb", syukatsuGame, field);

  enemyManager         = new CharacterManager("bbb", syukatsuGame, field);
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

  //今の所使っていない
  batcher = new SpriteBatcher(10);
  
  menuWindow = new IconList("iconList", syukatsuGame);
  MessageManager::initialize();
}

PlayScene::~PlayScene()
{
  glDisable(GL_LIGHTING);    
  glDisable(GL_LIGHT0);
  
  //全部解放
  root->setStatus(Actor::Dead);
  root->checkStatus();

  delete menuWindow;
  delete batcher;
  delete camera;
  delete menuCamera;  
}

void PlayScene::update(float deltaTime)
{
  Vector3 point;
  auto mouseEvent = syukatsuGame->getInput()->getMouseEvent();  
  Vector2 touch(mouseEvent->x, mouseEvent->y);
  Vector3 direction = camera->screenToWorld(touch);
  
  field->updateMousePosition(camera->getPosition(), direction);  //マウスが指しているフィールドのセルを更新

  Vector2 menuTouch = menuCamera->screenToWorld(touch);  //メニュー画面のタッチ位置
  
  Vector2 cell;
  //建物の建設
  if(mouseEvent->action == GLFW_PRESS || syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_C))
  {
    field->getMouseCollisionCell(cell);
    

    //新しい建物を建てる
    if(menuWindow->getSelectIcon() != -1 && field->isValidPosition(cell.x, cell.y))
    {
      int type = menuWindow->getSelectIcon();
      int baseValue = getBaseValueOfBuilding(type);

      if(baseValue <= playerManager->getGold()) { 
	auto building = getInstanceOfBuilding(menuWindow->getSelectIcon(), cell, syukatsuGame, field, enemyManager);
	playerBuildingManager->addChild(building);
	drawGoldString(building->getPosition(), -baseValue);
	playerManager->subGold(baseValue);	
      }
    }
    else if(field->getMouseCollisionCell(cell))
    {
      //今ある建物を選択する
      Building* building = field->getBuilding(cell.x, cell.y);

      if(building != NULL)
      {
	field->pickBuilding(cell.x, cell.y);
      }
      else {
	field->unPickedBuildingAll();
      }
    }

    menuWindow->selectIcon(menuTouch);    //メニュー画面の当たり判定をする
  }
  
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
      syukatsuGame->setScene(new ResultScene(syukatsuGame, ResultScene::DEFEATED));
    }

    //勝利
    if(remainEnemy <= 0) {
      syukatsuGame->setScene(new ResultScene(syukatsuGame, ResultScene::VICTORY));
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
  if(syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_L))
  {
    menuWindow->selectIcon(Information::LIGHTNING_TOWER);
  }
  else if(syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_F))
  {
    menuWindow->selectIcon(Information::FREEZING_TOWER);
  }

  
  //建物の削除
  if(syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_D) ||
     (mouseEvent->action == GLFW_PRESS && menuWindow->getTouchedButton(menuCamera->screenToWorld(touch)) == Information::DELETE_BUTTON ) ) {
    sellBuilding();
  }

  //建物のUpgrade
  if(syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_U) || 
     (mouseEvent->action == GLFW_PRESS && menuWindow->getTouchedButton(menuCamera->screenToWorld(touch)) == Information::UPGRADE_BUTTON )
    )
  {
    upgrading();
  }


  if(syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_P))
  {
    MessageManager::effectMessage("Debug", debug_character, 3);
  }
  //デバッグ情報
  Debugger::drawDebugInfo("PlayScene.cpp", "FPS", 1.0/deltaTime);
  Debugger::drawDebugInfo("PlayScene.cpp", "gold", playerManager->getGold());
  Debugger::drawDebugInfo("PlayScene.cpp", "enemy", remainEnemy);

  MessageManager::update(deltaTime);

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

  Vector2 cell;  
  if(menuWindow->getSelectIcon() != -1 && playerManager->getGold() >= 100 && field->getMouseCollisionCell(cell))
  {
    Debugger::drawDebugInfo("PlayScene.cpp", "menu", "menu");
    if(field->isValidPosition(cell.x, cell.y)) {
      Vector3 pos = field->cellToPoint(cell.x, cell.y);
      glPushAttrib(GL_COLOR_MATERIAL | GL_CURRENT_BIT | GL_ENABLE_BIT); 
      glPushMatrix();
      glTranslatef(pos.x, pos.y, pos.z);
      
      float col[] = {0.5, 1.0, 1.0, 0.3 };
      glMaterialfv(GL_FRONT, GL_AMBIENT, col);
      Assets::highLight->texture->bind();
      drawTexture(Vector3(0,2,0), Vector3(0,1,0), menuWindow->getSelectIconRange() * 2, Assets::highLight);
      glBindTexture(GL_TEXTURE_2D, 0);
      Assets::buildings[menuWindow->getSelectIcon()]->render(0.5);
      glPopMatrix();
      glPopAttrib();
    }
  }
  
  glPopAttrib();
  MessageManager::render(deltaTime, camera->getPosition());

  
  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
  glDisable(GL_DEPTH_TEST);  //これがあると2Dでは, 透過画像が使えないので消す
  glDisable(GL_LIGHTING);
  playCamera2D->setViewportAndMatrices();
//  MessageManager::render2(deltaTime, camera, playCamera2D);  
  glPopAttrib();

  
  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
  glDisable(GL_DEPTH_TEST);  //これがあると2Dでは, 透過画像が使えないので消す
  glDisable(GL_LIGHTING);
  menuCamera->setViewportAndMatrices();
  menuWindow->render(deltaTime);  
  Assets::textureAtlas->unbind();
  glPopAttrib();

  Debugger::renderDebug(syukatsuGame->getWindow());
}

void PlayScene::drawMenuString(int id, string name, const Vector3& pos)
{
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_COLOR_MATERIAL);

  glPushMatrix();
  glTranslatef(pos.x, pos.y, pos.z);
  glColor3d(1.0, 1.0, 1.0);
  
  Assets::mincho->render(name.c_str());

  glPopMatrix();

  glPopAttrib();
}

void PlayScene::drawGoldString(const Vector3& pos, int gold)
{
  std::stringstream ss;
  if(gold < 0) ss << "-";
  else ss << "+";

  ss << gold << "G";

  MessageManager::effectMessage(ss.str(), pos + Vector3(0,50,0), 1);
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

bool PlayScene::canUpgrade(Building* building) 
{
  if(building->isMaxLevel()) { return false; }
  
  return building->getUpgradeCost() <= playerManager->getGold();
}

void PlayScene::upgrading()
{
  Building* building = field->getPickedBuilding();
  if(building != NULL && canUpgrade(building)) {
    playerManager->subGold(building->getUpgradeCost());

    MessageManager::effectMessage("upgraded", building->getPosition() + Vector3(0,50,0), 1);
    building->upgrade();
  }
}

void PlayScene::sellBuilding()
{
  Building* building = field->getPickedBuilding();
  if(building != NULL) {
    //売却時に金銭を獲得
    int sellValue = building->getSellValue();
    drawGoldString(building->getPosition(), sellValue);  

    playerManager->addGold(building->getSellValue());
    field->deleteBuilding();
  }
}

int PlayScene::getBaseValueOfBuilding(int type)
{
  switch(type) {
  case Information::LIGHTNING_TOWER:
    return Information::BaseValues::LIGHTNING_BASE;
  case Information::FREEZING_TOWER:
    return Information::BaseValues::FREEZING_BASE;
  default:
    return -1;
  }
}
    

Building* PlayScene::getInstanceOfBuilding(int type, Vector2 cell, SyukatsuGame* game, Field *field, CharacterManager *cManager)
{
  Building *tower;
  switch(type) {
  case Information::LIGHTNING_TOWER:
    tower = new LightningTower("lightningTower", game, field, cManager);
    break;
    
  case Information::FREEZING_TOWER:
    tower = new FreezingTower("freezingTower", game, field, cManager);
    break;
    
  case Information::BARRACK:
    tower = new Barrack("barrack", game, field, cManager);
    break;
    
  default:
    return NULL;
  }
  field->setBuilding(tower, cell.x, cell.y);
  tower->setPosition(field->cellToPoint(cell.x, cell.y));
  tower->setPicked(false);
  return tower;  
}
