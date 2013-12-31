#ifndef PLAY_SCENE_H_12_23
#define PLAY_SCENE_H_12_23

#include <syukatsu/syukatsu.h>
#include "Actor.h"
#include "Character.h"
#include "Field.h"
#include "MouseMoveCamera.h"
#include "CharacterManager.h"

class PlayScene :public SyukatsuScene
{
  MouseMoveCamera *camera;
  Camera2D *menuCamera;
  SpriteBatcher *batcher;
  Actor *root;
  Field *field;  

  CharacterManager *playerManager, *enemyManager;
  int menuPos;

public:
  static constexpr float WIDTH  = 4.8f;
  static constexpr float HEIGHT = 3.2f;
  
  PlayScene(SyukatsuGame *Game);
  ~PlayScene();  

  void update(float deltaTime);
  void render(float deltaTime);
  void reshape(int width, int height)   {  }  
  void dispose()  {  }  

  void drawMenuString(int id, string name, const Vector3& pos);
};

#endif
