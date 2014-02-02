#ifndef __SYUKATSUDEFENDER_H__
#define __SYUKATSUDEFENDER_H__

#include <string>

static const int windowWidth = 640;
static const int windowHeight = 480;
static const string gameTitle = "SyukatsuDefender"; 

//mainで書き換えるのはこのクラスだけ
class SyukatsuDefender : public SyukatsuGame
{
public:
  SyukatsuDefender(GLFWwindow* window)
    :SyukatsuGame(window)
  {
    Assets::load();
    scene = getStartScene();
  }

  Scene* getStartScene()
  {
    return new TitleScene(this);    
  }
  
};

#endif
