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
  
  bool buildMode;

  static float MENU_WINDOW_WIDTH;
  static float MENU_WINDOW_HEIGHT;
public:
  static float getMenuWindowWidth();
  static float getMenuWindowHeight();    

  static constexpr float WIDTH  = 4.8f;
  static constexpr float HEIGHT = 3.2f;
  
  PlayScene(SyukatsuGame *Game);
  ~PlayScene();  

  void update(float deltaTime);
  void render(float deltaTime);
  void reshape(int width, int height)   {  }  
  void dispose()  {  }  

  void siege() { health -= 1; }

  void startWave(int waveNum);
  void decEnemyNum() { remainEnemy--; }

  void drawMenuString(int id, string name, const Vector3& pos);
};

#endif
