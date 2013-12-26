#ifndef PLAY_SCENE_H_12_23
#define PLAY_SCENE_H_12_23

#include <syukatsu/syukatsu.h>
#include "Actor.h"
#include "Character.h"
#include "Field.h"
#include "MouseMoveCamera.h"

class PlayScene :public SyukatsuScene
{
  MouseMoveCamera *camera;
  
  SpriteBatcher *batcher;
  Actor *root;
  Field *field;  
public:
  static constexpr float WIDTH  = 4.8f;
  static constexpr float HEIGHT = 3.2f;
  
  PlayScene(SyukatsuGame *Game);  

  void update(float deltaTime);
  void render(float deltaTime);
  void reshape(int width, int height)   {  }  
  void dispose()  {  }  
};

#endif
