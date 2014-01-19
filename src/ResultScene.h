#ifndef RESULT_SCHENE_H
#define RESULT_SCHENE_H

#include <syukatsu/syukatsu.h>

class ResultScene : public SyukatsuScene
{
public:
  typedef enum Result
  {
    VICTORY,
    DEFEATED
  } Result;
  
  ResultScene(SyukatsuGame *game, Result _result);
  ~ResultScene();
  void update(float deltaTime);
  void render(float deltaTIme);
  void reshape(int width, int height);
  void dispose();
private:
  float WIDTH;
  float HEIGHT;
  Camera2D *camera;
  SpriteBatcher *batcher;
  Result result;
};

#endif

