#ifndef __SYUKATSUDEFENDER_H__
#define __SYUKATSUDEFENDER_H__

#include <string>

static const int windowWidth  = 1080;
static const int windowHeight = 760;
static const string gameTitle = "SyukatsuDefender"; 

//mainで書き換えるのはこのクラスだけ
class SyukatsuDefender : public SyukatsuGame
{
public:
SyukatsuDefender(std::string title, int width, int height, bool is_fullscreen)
  :SyukatsuGame(title, width, height, is_fullscreen)
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
