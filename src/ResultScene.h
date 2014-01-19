#ifndef RESULT_SCHENE_H
#define RESULT_SCHENE_H

#include <syukatsu/syukatsu.h>

class ResultScene : public SyukatsuScene
{
  float WIDTH;
  float HEIGHT;
  Camera2D *camera;
  SpriteBatcher *batcher;  
public:
  ResultScene(SyukatsuGame *game);
  ~ResultScene();
  void update(float deltaTime);
  void render(float deltaTIme);
  void reshape(int width, int height);
  void dispose();  
};

#endif

