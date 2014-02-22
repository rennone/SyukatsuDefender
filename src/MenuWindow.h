#ifndef MENU_WINDOW_H_2013_01_16
#define MENU_WINDOW_H_2013_01_16

#include <syukatsu/syukatsu.h>
#include <math/Vector2.h>
#include "Information.h"
#include "Icon.h"
#include "Actor.h"
#include "Building.h"

class MenuWindow : public Actor
{
private:
  Camera2D     *camera;
  SpriteBatcher *batcher;
  Icon *towerIcons[Information::BUILDING_NUM];
  Icon *buttons[Information::BUTTON_NUM];  

  int select;  //選択しているアイコン : -1 -> 何も選択していない
  int push; //押しているボタン : -1 -> 何も押していない
  void setSelectedIcon(const Vector2 &touch);
  void setPushedButton(const Vector2 &touch);

public:
  MenuWindow( string text, SyukatsuGame *game, Camera2D *camera );
  ~MenuWindow();

  void update(float deltaTime);
  void render(float deltaTime);
  void reshape(int width, int height);

  void drawBuildingInfo(Building* building);

  int getSelectedIcon() const
  {
    return select;
  }

  int getAction() const
  {
    return push;
  }
 
  int getPressedButton(MouseEvent *event) const
  {
    Vector2 touch(event->x, event->y);
    touch = camera->screenToWorld(touch);
    for(int i=0; i<Information::BUTTON_NUM; i++)
    {
      if( !buttons[i]->inRegion(touch) )
        continue;

      return i;
    }
    return -1;
  }
};
#endif
