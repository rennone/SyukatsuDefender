#ifndef __SYUKATSUDEFENDER_H__
#define __SYUKATSUDEFENDER_H__

#include <string>

static const int windowWidth  = 960;
static const int windowHeight = 720;
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
