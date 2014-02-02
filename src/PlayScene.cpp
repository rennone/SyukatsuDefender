#include <syukatsu/GL/freeglut.h>
#include <sstream>
#include <cassert>
#include "Assets.h"
#include "Field.h"
#include "Character.h"
#include "PlayScene.h"
#include "TitleScene.h"
#include "SimpleObjectFactory.h"
#include "Barrack.h"
#include "LightningTower.h"
#include "FreezingTower.h"
#include "ArrowTower.h"
#include "TextBox.h"
#include "Debugger.h"
#include "TestCharacter.h"
#include "Information.h"
#include "Message.h"
#include "MessageManager.h"
#include "ResultScene.h"
#include "Soldier.h"
#include <sstream>

using namespace std;

float PlayScene::MENU_WINDOW_WIDTH;
float PlayScene::MENU_WINDOW_HEIGHT;
float PlayScene::PLAY_WINDOW_WIDTH;
float PlayScene::PLAY_WINDOW_HEIGHT;

Character *debug_character;

float PlayScene::getMenuWindowWidth()
{
  return MENU_WINDOW_WIDTH;
}

float PlayScene::getMenuWindowHeight()
{
  return MENU_WINDOW_HEIGHT;
}

float PlayScene::getPlayWindowWidth()
{
  return PLAY_WINDOW_WIDTH;
}

float PlayScene::getPlayWindowHeight()
{
  return PLAY_WINDOW_HEIGHT;
}

static void LightSetting()
{
  glEnable(GL_LIGHTING);    
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  glEnable(GL_LIGHT3);

  float edge = Field::cellNum*Field::cellSize*2;

  GLfloat lightpos1[] = { 0.0, edge/2, 0.0, 1.0 };
  GLfloat lightdir1[] = { 1.0, -1.0, 1.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos1);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightdir1);
  
  GLfloat lightpos2[] = { edge, edge/2, edge, 1.0 };
  GLfloat lightdir2[] = { -1.0, -1.0, -1.0, 1.0 };
  glLightfv(GL_LIGHT1, GL_POSITION, lightpos2);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightdir2);
 
  GLfloat lightpos3[] = { 0.0, edge/2, edge, 1.0 };
  GLfloat lightdir3[] = { 1.0, -1.0, -1.0, 1.0 };
  glLightfv(GL_LIGHT2, GL_POSITION, lightpos3);
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightdir3);
 
  GLfloat lightpos4[] = { edge, edge/2, 0.0, 1.0 };
  GLfloat lightdir4[] = { -1.0, -1.0, 1.0, 1.0 };
  glLightfv(GL_LIGHT4, GL_POSITION, lightpos4);
  glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, lightdir4); 
}

//2桁の数を描画
static void drawNumber(SpriteBatcher *batcher, Vector2 center, float size, int number)
{
  int dig1  = number%10;
  int dig10 = number/10;

  batcher->drawSprite( center.x, center.y , size, size, Assets::numbers[dig1]);

  if(dig10>0)
    batcher->drawSprite( center.x-size + size / 4, center.y , size, size, Assets::numbers[dig10]); 
}

PlayScene::PlayScene(SyukatsuGame *game, int stage)
  :SyukatsuScene(game)
  ,health(1)
  ,nowWave(1)
  ,buildMode(true)
  ,elapsedTime(0)
  ,buildPhaseTimer(BUILDING_TIME)
{
//カメラのViewport設定とか
  int width, height;
  glfwGetFramebufferSize(syukatsuGame->getWindow(), &width, &height);

  //とりあえず適当な値で初期化
  MENU_WINDOW_HEIGHT = MENU_WINDOW_WIDTH = 100;
  PLAY_WINDOW_HEIGHT = PLAY_WINDOW_WIDTH = 100;
  
  //横幅とかの設定はcameraViewportSettingでやる.
  camera  = new MouseMoveCamera(syukatsuGame, 1, 4000, 60);  
  menuCamera = new Camera2D(syukatsuGame->getWindow(), MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
  playCamera2D = new Camera2D(syukatsuGame->getWindow(), PLAY_WINDOW_WIDTH, PLAY_WINDOW_HEIGHT);  
  cameraViewportSetting(width, height);
  
  //全てのActorを一括してupdate, renderを行う為のルートアクター
  root = new Actor("root", syukatsuGame);
  
  field = new Field("field", syukatsuGame);    //フィールドの生成
  field->setLane(stage);
  
  root->addChild(field);                       //ルートアクターの子に追加

  //敵とプレイヤーの本拠地
  const Vector3 playerStronghold = field->cellToPoint(0,0);
  const Vector3 enemyStronghold  = field->cellToPoint(Field::cellNum-1, Field::cellNum-1);

  //全てのプレイヤーを管理するクラス
  playerManager         = new CharacterManager("aaa", syukatsuGame, field);
  playerBuildingManager = new CharacterManager("bbb", syukatsuGame, field);

  enemyManager          = new CharacterManager("bbb", syukatsuGame, field);
  enemyBuildingManager  = new CharacterManager("ccc", syukatsuGame, field);

  
  playerManager->setTarget(enemyStronghold - Vector3(10, 0, 0));
  enemyManager->setTarget(playerStronghold + Vector3(10, 0, 0));

  playerManager->setColor(Vector3(1.0, 0.0, 0.0));
  enemyManager->setColor(Vector3(0.0, 1.0, 0.0));

  //全てのエネミーを管理するクラス
  root->addChild(playerManager);
  root->addChild(enemyManager);

  root->addChild(playerBuildingManager);
  root->addChild(enemyBuildingManager);
    
  LightSetting();

  //Build Phaseとかのロゴ用
  batcher = new SpriteBatcher(10);
  
  menuWindow = new MenuWindow("menuWindow", syukatsuGame, menuCamera);
//  root->addChild(menuWindow);

}

PlayScene::~PlayScene()
{
  glDisable(GL_LIGHTING);    
  glDisable(GL_LIGHT0);

  //キャラクターを削除したときに, エフェクトメッセージがnull pointerにならないように, 先にリセット
  MessageManager::reset();
  //全部解放
  root->setStatus(Actor::Dead);
  root->checkStatus();

  delete batcher;
  delete camera;
  delete menuCamera;  
}

void PlayScene::cameraViewportSetting(int width, int height)
{  
  const int playWidth  = width*WINDOW_SPLIT_RATE;
  const int playHeight = height;
  const float playRatio = playWidth/(float)playHeight;
  
  const int menuWidth  = width-playWidth;
  const int menuHeight = height;
  const float menuRatio  = menuWidth/(float)menuHeight;

  MENU_WINDOW_HEIGHT = 100.0f;
  MENU_WINDOW_WIDTH  = MENU_WINDOW_HEIGHT*menuRatio;
  PLAY_WINDOW_HEIGHT = 400.0f;
  PLAY_WINDOW_WIDTH  = PLAY_WINDOW_HEIGHT*playRatio;
  
  //メニュー画面のカメラ
  menuCamera->setFrustumSize( Vector2(MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT));
  menuCamera->setViewportWidth(menuWidth);
  menuCamera->setViewportHeight(menuHeight);
  menuCamera->setViewportPosition(playWidth+menuWidth/2, menuHeight/2.0);

  //プレイ画面のカメラ
  camera->setViewportWidth(playWidth);
  camera->setViewportHeight(playHeight);
  camera->setViewportPosition(playWidth/2, playHeight/2);

  //プレイ画面のカメラ(2D)
  playCamera2D->setFrustumSize( Vector2(PLAY_WINDOW_WIDTH, PLAY_WINDOW_HEIGHT));
  playCamera2D->setViewportWidth(playWidth);
  playCamera2D->setViewportHeight(playHeight);
  playCamera2D->setViewportPosition(playWidth/2, playHeight/2); 
}

void PlayScene::reshape(int width, int height)
{
  cameraViewportSetting(width, height);
  menuWindow->reshape(width, height);
}

void PlayScene::update(float deltaTime)
{
  elapsedTime += deltaTime;

  //最初2秒はアニメーションだけ
  if( elapsedTime < START_ANIMATION_TIME)
  {
    camera->rotate(M_PI*deltaTime/START_ANIMATION_TIME, 0);
    camera->zoom(500*deltaTime/START_ANIMATION_TIME);
    return;
  }
  
  if(buildMode)
  {
    //建設中
    buildPhaseTimer -= deltaTime;
    
    if(syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_S) || buildPhaseTimer <= 0)
    {
      startWave(nowWave);
      buildMode = false;
    }
  }
  else
  {
    //ゲーム終了
    //敗北
    if(health <= 0)
    {
      syukatsuGame->setScene(new ResultScene(syukatsuGame, ResultScene::DEFEATED, nowWave, elapsedTime));
    }
    
    //wave clear
    if(remainEnemy <= 0) {
      if(nowWave >= 5)
      {
	syukatsuGame->setScene(new ResultScene(syukatsuGame, ResultScene::VICTORY, nowWave, elapsedTime));
      }
      else {
	buildMode = true;
	nowWave++;
	buildPhaseTimer = BUILDING_TIME;
      }
    }
  }

  auto keyEvents = syukatsuGame->getInput()->getKeyEvents();
  auto mouseEvent = syukatsuGame->getInput()->getMouseEvent();
  
  Vector2 touch(mouseEvent->x, mouseEvent->y);
  Vector3 direction = camera->screenToWorld(touch);  
  field->updateMousePosition(camera->getPosition(), direction);  //マウスが指しているフィールドのセルを更新
  
  //デバッグ エンターでタイトルに戻る
  if (syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_ENTER))
  {
    MessageManager::reset();
    syukatsuGame->setScene(new TitleScene(syukatsuGame));
    return;
  }

  //マウスが指しているセルを求める : pointMap=>指しているかどうか
  Vector2 cell;
  const bool pointMap = field->getMouseCollisionCell(cell);

  //メニューのアイコンを選択している時は, 建物のpickedを消す
  if ( menuWindow->getSelectedIcon() != -1 )
    field->unPickedBuildingAll();
  
  if( mouseEvent->action == GLFW_PRESS )
  { 
  //建物の建設
    if( pointMap && menuWindow->getSelectedIcon() != -1 && field->isBuildable(cell.x, cell.y))
    {
      int type = menuWindow->getSelectedIcon();
      int baseValue = getBaseValueOfBuilding(type);

      if(baseValue <= playerManager->getGold())
      { 
        auto building = getInstanceOfBuilding( type, cell, syukatsuGame, field, enemyManager);
        playerBuildingManager->addChild(building);
        drawGoldString(building->getPosition(), -baseValue);
        playerManager->subGold(baseValue);	
      }         
    }
    else if(pointMap)
    {
      field->pickBuilding(cell.x, cell.y); //フィールドの選択点の更新
      //プレイヤーによる攻撃
      if(menuWindow->getSelectedIcon() == -1 && !buildMode && field->getPickedBuilding() == NULL) {
	Character* target = NULL;
        float mindist = 30;
	for(auto c : enemyManager->getChildren()) {
          Vector3 dist = ((Character *)c)->getPosition() - field->getMousePoint();
	  if(dist.length() < mindist) {
	    mindist = dist.length();
	    
	    target = (Character *)c;
	  }
	}

	if(target != NULL) {
	  target->gotDamage(10000);
	}
	puts("attack");
      }
    }
  }

  // 建設の後じゃないと, 上手く動かない
  // 連続して置けるようにと, 既に置いている場所を間違ってクリックしたときに, 選択が解けないようにする条件式
//  if ( !( mouseEvent->action == GLFW_PRESS && pointMap && menuWindow->getSelectedIcon() != -1 && !field->isBuildable(cell.x, cell.y) ) )  
    menuWindow->update(deltaTime);


  Debugger::drawDebugInfo("PlayScene.cpp", "action", menuWindow->getAction());
    
  //建物の削除
  if( menuWindow->getAction() == Information::DELETE_BUTTON )
    sellBuilding();  

  //建物のUpgrade
  if( menuWindow->getAction() == Information::UPGRADE_BUTTON )
    upgrading();

  //characterのアップデートもまとめて行われる
  root->update(deltaTime);
  root->checkStatus();
  
  //エフェクトメッセージの位置を更新
  MessageManager::update(deltaTime);

    //カメラのアップデード
  camera->mouseTrack();

  Debugger::drawDebugInfo("PlayScene.cpp", "FPS", 1.0/deltaTime);
  Debugger::drawDebugInfo("PlayScene.cpp", "Wave No.", nowWave);
  Debugger::drawDebugInfo("PlayScene.cpp", "gold", playerManager->getGold());
  Debugger::drawDebugInfo("PlayScene.cpp", "enemy", remainEnemy);
}

static void setting2D()
{
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);  //これがあると2Dでは, 透過画像が使えないので消す
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
}

void PlayScene::render(float deltaTime)
{
  //アクションウィンドウの描画
  actionWindowRender(deltaTime);

  //アクションウィンドウに重ねる2D画面の描画
  actionWindowOverlapRender(deltaTime);

  //メニューウィンドウの描画
  menuWindowRender(deltaTime);
}

void PlayScene::menuWindowRender(float deltaTime)
{
  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);
  glPushMatrix();
  setting2D();
  MessageManager::render2DMessage(deltaTime);
  menuCamera->setViewportAndMatrices();
  menuWindow->render(deltaTime);
  //デバッグ情報の描画
  Debugger::renderDebug(syukatsuGame->getWindow());
  glPopMatrix();
  glPopAttrib();
}

//アクションウィンドの上に載せる2D部分の描画
void PlayScene::actionWindowOverlapRender(float deltaTime)
{
  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);  
  setting2D();
  
  glPushMatrix();
  
  playCamera2D->setViewportAndMatrices();
  glColor4f(1,1,1,1);
  batcher->beginBatch(Assets::playAtlas);

  if(buildMode)
  {
    //建設中
    if(elapsedTime < START_ANIMATION_TIME)
    {
      //最初のアニメーション中
      const float m = 1.5;
      const float b = M_PI-asin(1.0/m);
      const float ratio = m*sin(elapsedTime/START_ANIMATION_TIME*b);
      batcher->drawSprite( 0,  PLAY_WINDOW_HEIGHT*0.4, ratio*PLAY_WINDOW_WIDTH/2, ratio*PLAY_WINDOW_HEIGHT/4, Assets::buildPhase);
    }
    else
    {
      if(buildPhaseTimer <= 3)
        glColor4f(1,1,1, pow(1-sin(30*buildPhaseTimer),2));
      batcher->drawSprite( 0,  PLAY_WINDOW_HEIGHT*0.4, PLAY_WINDOW_WIDTH/2, PLAY_WINDOW_HEIGHT/4, Assets::buildPhase);
      
      float _size = PLAY_WINDOW_WIDTH/10;
      drawNumber( batcher, Vector2( (PLAY_WINDOW_WIDTH-_size)/2, PLAY_WINDOW_HEIGHT*0.4), _size, buildPhaseTimer+1 );
    }
  }
  else
  {
    batcher->drawSprite( 0, PLAY_WINDOW_HEIGHT*0.4,
                         PLAY_WINDOW_WIDTH/2, PLAY_WINDOW_HEIGHT/4,
                         Assets::battlePhase);
    int _size = PLAY_WINDOW_WIDTH/10;
    drawNumber( batcher, Vector2( -PLAY_WINDOW_WIDTH/2+2*_size, PLAY_WINDOW_HEIGHT*0.4), _size, remainEnemy);
  }
  batcher->endBatch();
  
  glPopMatrix();
  glPopAttrib();  
}

void PlayScene::actionWindowRender(float deltaTime)
{
  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  glEnable(GL_LIGHT3);
  
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  
  camera->setViewportAndMatricesWithMouse();

  root->render(deltaTime);  //全てのキャラクターの描画

  //選択している建物の描画
  Vector2 cell;
  bool pointMap = field->getMouseCollisionCell(cell);
  
  if ( menuWindow->getSelectedIcon() != -1 && playerManager->getGold() >= 100 && pointMap )
  {    
    Vector3 pos = field->cellToPoint(cell.x, cell.y);
    glTranslatef(pos.x, pos.y, pos.z);

    Assets::playAtlas->bind();
    if(field->isBuildable(cell.x, cell.y))
      drawTexture( Vector3(0,2,0), Vector3(0,1,0), Information::DefaultRangeOfBuildings[menuWindow->getSelectedIcon()]*2, Assets::greenRange);
    else
      drawTexture( Vector3(0,2,0), Vector3(0,1,0), Information::DefaultRangeOfBuildings[menuWindow->getSelectedIcon()]*2, Assets::redRange);
    glBindTexture(GL_TEXTURE_2D, 0);
    Assets::buildings[menuWindow->getSelectedIcon()]->render(0.5);   
  }
  
  glPopAttrib();
  
  MessageManager::render3DMessage(deltaTime, camera->getPosition());
  
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
  if(building != NULL && canUpgrade(building))
  {
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
  case Information::ARROW_TOWER:
    return Information::BaseValues::ARROW_BASE;
  default:
    printf("baseValue is not found, type = %d\n", type);
    assert(false);
    return -1;
  }
}
    

Building* PlayScene::getInstanceOfBuilding(int type, Vector2 cell, SyukatsuGame* game, Field *field, CharacterManager *cManager)
{
  Building *tower;
  switch(type)
  {
  case Information::LIGHTNING_TOWER:
    tower = new LightningTower("lightningTower", game, field, cManager);
    break;
    
  case Information::FREEZING_TOWER:
    tower = new FreezingTower("freezingTower", game, field, cManager);
    break;

  case Information::ARROW_TOWER:
    tower = new ArrowTower("arrowTower", game, field, cManager);
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
