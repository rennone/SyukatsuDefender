#ifndef PLAY_SCENE_H_12_23
#define PLAY_SCENE_H_12_23

#include <syukatsu/syukatsu.h>
#include "Actor.h"
#include "Character.h"
#include "Field.h"
#include "MouseMoveCamera.h"
#include "CharacterManager.h"
#include "IconList.h"

class PlayScene : public SyukatsuScene
{
  MouseMoveCamera *camera;
  Camera2D *playCamera2D;
  Camera2D *menuCamera;
  SpriteBatcher *batcher;  
  IconList *menuWindow;  
  Actor *root;
  Field *field;  

  CharacterManager *playerManager, *enemyManager;
  CharacterManager *playerBuildingManager, *enemyBuildingManager;

  int health;
  int nowWave;
  int remainEnemy;

  //建設フェイズ残り時間
  int buildPhaseTimer;
  
  bool buildMode;

  static float MENU_WINDOW_WIDTH;
  static float MENU_WINDOW_HEIGHT;
  
  static float PLAY_WINDOW_WIDTH;
  static float PLAY_WINDOW_HEIGHT;
public:
  static float getMenuWindowWidth();
  static float getMenuWindowHeight();
  
  static float getPlayWindowWidth();
  static float getPlayWindowHeight();    
  
  PlayScene(SyukatsuGame *Game);
  ~PlayScene();  

  void update(float deltaTime);
  void render(float deltaTime);
  void reshape(int width, int height)   {  }  
  void dispose()  {  }  

  void siege() { health -= 1; }

  //建物関連
  bool canUpgrade(Building* build);
  Building* getInstanceOfBuilding(int type, Vector2 cell, SyukatsuGame* game, Field *field, CharacterManager *cManager);
  int getBaseValueOfBuilding(int type);
  
  //プレイヤーの行動
  void upgrading();
  void sellBuilding();

  void startWave(int waveNum);
  void decEnemyNum() { remainEnemy--; }
  void addGold(int value) { playerManager->addGold(value); }

  void drawMenuString(int id, string name, const Vector3& pos);
  void drawGoldString(const Vector3& pos, int value);

  Vector3 getCameraPosition() const
  {
    return camera->getPosition();
  }
};

#endif
