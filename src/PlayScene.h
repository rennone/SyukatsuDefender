#ifndef PLAY_SCENE_H_12_23
#define PLAY_SCENE_H_12_23

#include <syukatsu/syukatsu.h>
#include "Actor.h"
#include "Character.h"
#include "Field.h"
#include "MouseMoveCamera.h"
#include "CharacterManager.h"
#include "Icon.h"
#include "MenuWindow.h"
#include "StrongHold.h"
#include "Player.h"

class PlayScene : public SyukatsuScene
{
private:
  MouseMoveCamera *camera;
  Camera2D *playCamera2D;
  Camera2D *menuCamera;
  SpriteBatcher *batcher;
  MenuWindow *menuWindow;
  Actor *root;
  Field *field;
  StrongHold *strongHold;
  CharacterManager *playerManager, *enemyManager;
  CharacterManager *playerBuildingManager, *enemyBuildingManager;
  
  void (PlayScene::*renderFunction)(float); //update用関数  
//  void startAnimationRender(float deltaTime); //最初のアニメーション
  void playRender(float deltaTime);
  void actionWindowRender(float deltaTime);
  void actionWindowOverlapRender(float deltaTime);
  void menuWindowRender(float deltaTime);

  Player *player;
  
  int nowWave;
  int remainEnemy;

  float elapsedTime; //総経過時間
  
  //建設フェイズ残り時間
  float buildPhaseTimer;
  
  bool buildMode;  
  static constexpr float WINDOW_SPLIT_RATE = 3.0/4.0;    
  static constexpr float BUILDING_TIME = 10.0; //建設タイムは30秒
  static constexpr float START_ANIMATION_TIME = 2.0f;
  static float MENU_WINDOW_WIDTH;
  static float MENU_WINDOW_HEIGHT;  
  static float PLAY_WINDOW_WIDTH;
  static float PLAY_WINDOW_HEIGHT;

  //画面のサイズ width, heightの時のカメラの設定
  void cameraViewportSetting(int width, int height);

  void drawLogo();

  void clickedAction(MouseEvent *event); //クリック時の処理
public:
  static float getMenuWindowWidth();
  static float getMenuWindowHeight();
  
  static float getPlayWindowWidth();
  static float getPlayWindowHeight();    
  
  PlayScene(SyukatsuGame *Game, int stage);
  ~PlayScene();  

  void update(float deltaTime);
  void render(float deltaTime);
  void reshape(int width, int height);
  void dispose()  {}  

  void siege() { strongHold->siege(); }

  //建物関連
  bool canUpgrade(Building* build);
  Building* getInstanceOfBuilding(int type, Vector2 cell, SyukatsuGame* game, Field *field, CharacterManager *cManager);
  int getBaseValueOfBuilding(int type);
  
  //プレイヤーの行動
  void upgrading();
  void sellBuilding();

  void startWave(int waveNum);
  void decEnemyNum() { remainEnemy--; }
  void addGold(int value) { player->addGold(value); }

  void drawMenuString(int id, string name, const Vector3& pos);
  void drawGoldString(const Vector3& pos, int value);

  Player* getPlayer() { return player; }

  Vector3 getCameraPosition() const
  {
    return camera->getPosition();
  }
};

#endif
