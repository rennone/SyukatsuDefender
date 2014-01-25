#ifndef MENU_WINDOW_H_2013_01_16
#define MENU_WINDOW_H_2013_01_16

#include <syukatsu/syukatsu.h>
#include <math/Vector2.h>
#include "Information.h"
#include "Icon.h"
#include "Actor.h"
class MenuWindow : public Actor
{
Camera2D     *camera;
SpriteBatcher *batcher;
  Icon *towerIcons[Information::BUILDING_NUM];
  Icon *buttons[Information::BUTTON_NUM];  
  int page;    //今見ているページ
  int select;  //選択しているアイコン : -1 -> 何も選択していない
int push; //押しているボタン : -1 -> 何も押していない
  void setSelectedIcon(const Vector2 &touch);
void setPushedButton(const Vector2 &touch);
public :
MenuWindow( string text, SyukatsuGame *game, Camera2D *camera );
  ~MenuWindow();

void update(float deltaTime);
void render(float deltaTime);
void reshape(int width, int height);

int getSelectedIcon() const
{
return select;
}

  int getAction() const
  {
return push;
}
};
#endif
