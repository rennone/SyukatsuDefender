#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include <syukatsu/syukatsu.h>
#include "Assets.h"

class TestScene : public Scene
{
  static constexpr float WIDTH = 4.8f;
  static constexpr float HEIGHT = 3.2f;
  Camera3D *camera;
  SpriteBatcher *batcher;  
public:
  TitleScene(SyukatsuGame *game);
  ~TitleScene();
  
  void update(float deltaTime);
  void render(float deltaTIme);
  void reshape(int width, int height);
  void dispose();    
};

#endif
