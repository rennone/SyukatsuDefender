#ifndef TITLE_SCENE_H_12_22
#define TITLE_SCENE_H_12_22

#include <syukatsu/syukatsu.h>

class TitleScene:public SyukatsuScene
{
  static constexpr float WIDTH = 4.8f;
  static constexpr float HEIGHT = 3.2f;
  Camera2D *camera;
  SpriteBatcher *batcher;
  float elapsedTime;
public:
  TitleScene(SyukatsuGame *game);
  ~TitleScene();
  
  void update(float deltaTime);
  void render(float deltaTIme);
  void reshape(int width, int height);
  void dispose();  
};


#endif
