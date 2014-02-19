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
  
  ResultScene(SyukatsuGame *game, Result result, int waveNum, float defenseTime);
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
  float defenseTime;
  float elapsedTime;
  int waveNum;
};

#endif

