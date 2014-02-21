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

#include "Debugger.h"
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

//光源の設定
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

//n桁の数を描画
static void drawNumber(SpriteBatcher *batcher, Vector2 center, float size, int number)
{
  int digits[20];
  int digNum = 0;

  while(number >= 0) {
    digits[digNum] = number % 10;
    ++digNum;
    number /= 10;

    if(number == 0) break;
  }

  for(int i = digNum - 1, j = 0; i >= 0; --i, ++j) {
    batcher->drawSprite(center.x + (size - size / 4) * j, center.y, size, size, Assets::numbers[digits[i]]);
  }
}

PlayScene::PlayScene(SyukatsuGame *game, int stage)
  :SyukatsuScene(game)
  ,nowWave(1)
  ,maxWave(5)
  ,elapsedTime(0)
  ,buildPhaseTimer(BUILDING_TIME)
  ,buildMode(true)
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

  strongHold = new StrongHold("strongHold", syukatsuGame, field);
  root->addChild(strongHold);
  //敵とプレイヤーの本拠地
  const Vector3 playerStronghold = field->cellToPoint(0,0);
  const Vector3 enemyStronghold  = field->cellToPoint(Field::cellNum-1, Field::cellNum-1);

  const int initialGold = 10000;
  player = new Player("player", syukatsuGame, initialGold);
  root->addChild(player);

  //全てのプレイヤーを管理するクラス
  playerManager         = new CharacterManager("aaa", syukatsuGame, field);
  playerBuildingManager = new CharacterManager("bbb", syukatsuGame, field);

  enemyManager          = new CharacterManager("bbb", syukatsuGame, field);
  enemyBuildingManager  = new CharacterManager("ccc", syukatsuGame, field);

  //全てのエネミーを管理するクラス
  root->addChild(playerManager);
  root->addChild(enemyManager);

  root->addChild(playerBuildingManager);
  root->addChild(enemyBuildingManager);
    
  LightSetting();

  //Build Phaseとかのロゴ用
  batcher = new SpriteBatcher(100);
  
  menuWindow = new MenuWindow("menuWindow", syukatsuGame, menuCamera);
}

PlayScene::~PlayScene()
{
  glDisable(GL_LIGHTING);    
  glDisable(GL_LIGHT0);

  //キャラクターを削除したときに, エフェクトメッセージがnull pointerにならないように, 先にリセット
  MessageManager::getInstance()->reset();
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

  //最初のSTART_ANIMATION_TIMEはアニメーションだけ
  if( elapsedTime < START_ANIMATION_TIME)
  {
    camera->rotate(M_PI*deltaTime/START_ANIMATION_TIME, 0);
    camera->zoom(500*deltaTime/START_ANIMATION_TIME);
    return;
  }

  auto keyEvents = syukatsuGame->getInput()->getKeyEvents();
  auto mouseEvent = syukatsuGame->getInput()->getMouseEvent();
  
  Vector2 touch(mouseEvent->x, mouseEvent->y);
  Vector3 direction = camera->screenToWorld(touch);
  
  //カメラの移動や回転
  camera->mouseTrack(deltaTime);
  
  //マウスが指しているフィールドのセルを更新
  if ( camera->inWindow(touch) )
  {    
    field->updateMousePosition(camera->getPosition(), direction);
  }
  else
  {
    //マウスがメニュー画面の上にある場合は,強制的にfalseにする.
    //ズームしたときに, メニュー画面尾上に合っても, フィールドを指している場合があるのでそれ対策
    field->mouseNotInRegion();
  }       

  if(buildMode)
  {
    //建設中
    buildPhaseTimer -= deltaTime;
    
    if(syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_SPACE) || buildPhaseTimer <= 0)
    {
      startWave(nowWave);
      buildMode = false;
    }
  }
  else
  {
    //ゲーム終了
    //敗北
    if(strongHold->destroyed())
    {
      syukatsuGame->setScene(new ResultScene(syukatsuGame, ResultScene::DEFEATED, nowWave, elapsedTime));
    }
    
    //wave clear
    if(remainEnemy <= 0) {
      if(nowWave >= maxWave)
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

#ifdef DEBUG
  //デバッグ エンターでタイトルに戻る
  if (syukatsuGame->getInput()->isKeyPressed(GLFW_KEY_ENTER))
  {
    MessageManager::getInstance()->reset();
    syukatsuGame->setScene(new TitleScene(syukatsuGame));
    return;
  }
#endif
  
  //メニューのアイコンを選択している時は, 建物のpickedを消す
  if ( menuWindow->getSelectedIcon() != -1 )
    field->unPickedBuildingAll();

  //クリックした時の処理
  if( mouseEvent->action == GLFW_PRESS )
    clickedAction(mouseEvent);  

  //建設処理の後じゃないといけない
  menuWindow->update(deltaTime);

//建物の削除
  if( menuWindow->getAction() == Information::DELETE_BUTTON )
    sellBuilding();
  
  //建物のアップグレード
  if( menuWindow->getAction() == Information::UPGRADE_BUTTON )
    upgrading();
      
  //characterやbuildingのアップデート
  root->update(deltaTime);
  root->checkStatus();
  
  //エフェクトメッセージの位置をアップデート
  MessageManager::getInstance()->update(deltaTime);

  Debugger::drawDebugInfo("PlayScene.cpp", "touchPoint", touch);  
  Debugger::drawDebugInfo("PlayScene.cpp", "FPS", 1.0/deltaTime);
  Debugger::drawDebugInfo("PlayScene.cpp", "enemy", remainEnemy);
}

void PlayScene::clickedAction(MouseEvent *event)
{  
  //マウスが指しているセルを求める : pointMap=>指しているかどうか
  Vector2 cell;
  const bool isMapPointing = field->getMouseCollisionCell(cell);
  //建物の建設
  if( isMapPointing && menuWindow->getSelectedIcon() != -1 && field->isBuildable(cell.x, cell.y))
  {
    int type = menuWindow->getSelectedIcon();
    int baseValue = getBaseValueOfBuilding(type);

    if(baseValue <= player->getGold())
    { 
      auto building = getInstanceOfBuilding( type, cell, syukatsuGame, field, enemyManager);
      playerBuildingManager->addChild(building);
      drawGoldString(building->getPosition(), -baseValue);
      player->subGold(baseValue);
    }     
  }
  else if( isMapPointing )
  {
    field->pickBuilding(cell.x, cell.y); //フィールドの選択点の更新      
    //プレイヤーによる攻撃
    if(menuWindow->getSelectedIcon() == -1 && !buildMode && field->getPickedBuilding() == NULL ) {
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
	if(player->canMagicAttack()) {
	  target->gotDamage(10000);
	  player->castFireball(target->getPosition());
	}
	else {
	  MessageManager::getInstance()->effectMessage("No Mana", Vector2(0,0), 1);
	}
      }
    }
  }
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

//メニューウィンドウの描画
void PlayScene::menuWindowRender(float deltaTime)
{
  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);
  glPushMatrix();
  setting2D();
  stringstream ss;
  ss.str(""); // バッファをクリアする。
  ss.clear(stringstream::goodbit);// ストリームの状態をクリアする。この行がないと意図通りに動作しない
  ss << "time " << elapsedTime;
  
  menuCamera->setViewportAndMatrices();
  menuWindow->render(deltaTime);
  Debugger::renderDebug(syukatsuGame->getWindow());  //デバッグ情報の描画
  glPopMatrix();
  glPopAttrib();
}

//アクションウィンドの上に載せる2D部分の描画
void PlayScene::actionWindowOverlapRender(float deltaTime)
{  
  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);  
  setting2D();  //デプステストとかの解除
  
  glPushMatrix();  
  playCamera2D->setViewportAndMatrices();
  glColor4f(1,1,1,1);

  const float PhaseMessageWidth  = PLAY_WINDOW_WIDTH/2;                        //フェーズ文字の幅
  const float PhaseMessageHeight = Assets::buildPhase->ratio*PhaseMessageWidth; //buildPhaseもbattlePhaseも元の大きさは同じなので先に計算
  const float PhaseMessageX      = (-PLAY_WINDOW_WIDTH + PhaseMessageWidth)*0.5; //
  const float PhaseMessageY      = (PLAY_WINDOW_HEIGHT-PhaseMessageHeight)*0.5;  //フェーズ文字の位置 

  if(buildMode)
  {
    //建設中
    if(elapsedTime < START_ANIMATION_TIME)
    {
      //開始時のアニメーション(アップになる)
      const float m = 1.5;
      const float b = M_PI-asin(1.0/m);
      const float p = elapsedTime/START_ANIMATION_TIME;
      const float ratioB = m*sin(p*b);

      batcher->beginBatch(Assets::playAtlas);
      batcher->drawSprite( PLAY_WINDOW_WIDTH/4*(1-p) + p*PhaseMessageX,  PhaseMessageY,
                           ratioB*PhaseMessageWidth, ratioB*PhaseMessageHeight, Assets::buildPhase);
      batcher->endBatch();
    }
    else
    {      
      //残り時間が3秒以下だと点滅
      if(buildPhaseTimer <= 3)
        glColor4f(1,1,1, pow(1-sin(30*buildPhaseTimer),2));
      
      batcher->beginBatch(Assets::playAtlas);
      batcher->drawSprite( PhaseMessageX,  PhaseMessageY, PhaseMessageWidth, PhaseMessageHeight, Assets::buildPhase);
      batcher->endBatch();
      glColor4f(1,1,1, 1);

      float _size = PLAY_WINDOW_WIDTH/10;
      const float TimerX = +PLAY_WINDOW_WIDTH/2  - _size*2;
      const float TimerY = +PLAY_WINDOW_HEIGHT/2 - _size;
      stringstream ss;
      ss << buildPhaseTimer+1;
      batcher->beginBatch(Assets::playAtlas);
      drawNumber( batcher, Vector2( TimerX, TimerY), _size, buildPhaseTimer+1 );
      batcher->endBatch();
    }
  }
  else
  {
    batcher->beginBatch(Assets::playAtlas);
    batcher->drawSprite( PhaseMessageX, PhaseMessageY, PhaseMessageWidth, PhaseMessageHeight,  Assets::battlePhase);
    batcher->endBatch();
  }
  
  //ユーザー情報の描画
  const float CharSize = PLAY_WINDOW_WIDTH/20;  //文字の大きさ
  const float InfoMessageX = 0;
  const float InfoMessageY = -PLAY_WINDOW_HEIGHT/2+6*CharSize;

  //フレームの描画
  MessageManager::drawFrame( Vector2(InfoMessageX, InfoMessageY), Vector2(PLAY_WINDOW_WIDTH/2, 6*CharSize));

  std::stringstream ss;
  //撃破数
  ss << "Wave " << nowWave << " of " << maxWave;
  MessageManager::drawBitmapString(ss.str(), Vector2(CharSize, InfoMessageY - 2*CharSize), CharSize*0.8);
  ss.str("");
  ss.clear(stringstream::goodbit);// ストリームの状態をクリアする。この行がないと意図通りに動作しない

  //金の描画
  ss << "Gold:" << player->getGold();
  MessageManager::drawBitmapString(ss.str(), Vector2(CharSize, InfoMessageY - 3*CharSize), CharSize, TextColors::YellowText);  
  ss.str(""); // バッファをクリアする。
  ss.clear(stringstream::goodbit);// ストリームの状態をクリアする。この行がないと意図通りに動作しない

  //ライフの描画
  ss << "Life:" << (int)strongHold->getHealth();
  MessageManager::drawBitmapString(ss.str(), Vector2(CharSize, InfoMessageY - 4*CharSize), CharSize);
  ss.str(""); // バッファをクリアする。
  ss.clear(stringstream::goodbit);// ストリームの状態をクリアする。この行がないと意図通りに動作しない

  //マナの描画
  ss << "Mana:" << (int)player->getMana();
  MessageManager::drawBitmapString(ss.str(), Vector2(CharSize, InfoMessageY - 5*CharSize), CharSize, TextColors::GreenText);
  ss.str(""); // バッファをクリアする。
  ss.clear(stringstream::goodbit);// ストリームの状態をクリアする。この行がないと意図通りに動作しない

  MessageManager::getInstance()->render2DMessage(deltaTime);
  
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
  
  camera->setViewportAndMatrices();
  root->render(deltaTime);  //全てのキャラクターの描画

  //選択している建物の描画
  Vector2 cell;
  bool pointMap = field->getMouseCollisionCell(cell);
  Assets::playAtlas->bind();
  if(pointMap)
  {
    glPushMatrix();
    Vector3 pos = field->cellToPoint(cell.x, cell.y);
    glTranslatef(pos.x, pos.y, pos.z);
    if ( menuWindow->getSelectedIcon() != -1 && player->getGold() >= 100)
    {
      BuildingBaseStatus* baseStatus = field->getBaseStatus()->getBuildingBaseStatus(menuWindow->getSelectedIcon());
      if(field->isBuildable(cell.x, cell.y))
      {
        drawTexture( Vector3(0,2,0), Vector3(0,1,0), baseStatus->getRangeOfEffect()*2, Assets::greenRange);
      }
      else {
        drawTexture( Vector3(0,2,0), Vector3(0,1,0), baseStatus->getRangeOfEffect()*2, Assets::redRange);
      }
      glBindTexture(GL_TEXTURE_2D, 0);
      Assets::buildings[menuWindow->getSelectedIcon()]->render(0.5);
    }
    else
    {
      glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT);
      glDisable(GL_LIGHTING);
      glDisable(GL_DEPTH_TEST);

      glColor4f(1,1,1,0.5f);
      if(field->isBuildable(cell.x, cell.y))
        drawTexture( Vector3(0,2,0), Vector3(0,1,0), Field::cellSize, Assets::buildable);
      else
        drawTexture( Vector3(0,2,0), Vector3(0,1,0), Field::cellSize, Assets::unBuildable);
      glBindTexture(GL_TEXTURE_2D, 0);
      glPopAttrib();
    }
    glPopMatrix();
  }

  MessageManager::getInstance()->render3DMessage(deltaTime, camera->getPosition(), camera->getLook());
  glPopAttrib();  
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
  if(gold < 0)
    ss << gold << "G";
  else
    ss << "+" << gold << "G";

  MessageManager::getInstance()->effectMessage(ss.str(), pos + Vector3(0,50,0), 1);
}

void PlayScene::startWave(int waveNum) 
{
  remainEnemy = 10;
  nowWave = waveNum;

  const Vector3 enemyStronghold = Vector3(field->getPosition()+field->getSize()-Vector3(10,0,10));

  auto ebarrack = new Barrack("barrack2", syukatsuGame, field, enemyManager);
  ebarrack->setPosition(enemyStronghold);

  enemyBuildingManager->addChild(ebarrack);

  //仮置きの建物を固定
  field->fixBuilding();
}

bool PlayScene::canUpgrade(Building* building) 
{
  if(building->isMaxLevel()) { return false; }
  
  return building->getUpgradeCost() <= player->getGold();
}

void PlayScene::upgrading()
{
  Building* building = field->getPickedBuilding();
  if(building != NULL && canUpgrade(building))
  {
    player->subGold(building->getUpgradeCost());

    MessageManager::getInstance()->effectMessage("upgraded", building->getPosition() + Vector3(0,50,0), 1);
    building->upgrade();
  }
}

void PlayScene::sellBuilding()
{
  Building* building = field->getPickedBuilding();
  if(building != NULL)
  {
    //売却時に金銭を獲得
    int sellValue = building->getSellValue();
    drawGoldString(building->getPosition(), sellValue);  
    player->addGold(building->getSellValue());
    field->deleteBuilding();
  }
}

int PlayScene::getBaseValueOfBuilding(int type)
{
  BuildingBaseStatus* baseStatus = field->getBaseStatus()->getBuildingBaseStatus(type);

  return baseStatus->getBaseValue();
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
  if(!buildMode) {
    tower->setFixed(true);
  }

  return tower;  
}
