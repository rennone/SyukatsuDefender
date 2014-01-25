#ifndef ICON_LIST_H
#define ICON_LIST_H

#include "Actor.h"
#include <math/Vector2.h>
#include "Information.h"
#include "Icon.h"

class SpriteBatcher;
class Building;

#include "Information.h"

class IconList : public Actor
{
  Icon *icons[Information::BUILDING_NUM];    
  Icon *buttons[Information::BUTTON_NUM];

  Building* buildings[Information::BUILDING_NUM];
  
  int page;    //今見ているページ
  int select;  //選択しているアイコン : -1 -> 何も選択していない
  int button;
  int iconNum; //現在のアイコンの数
  float iconSize; //アイコンのサイズ
  SpriteBatcher *batcher;
public: 
    IconList(string name, SyukatsuGame *game);
    ~IconList();
    void render(float deltaTime);    
        
    bool selectIcon(const Vector2 &touch);
    bool selectIcon(const int type);
    int getSelectIcon() const;
    float getSelectIconRange() const;

    Building* getSelectIconBuilding() const;

    //delete, upgradeButton用
    int getTouchedButton(const Vector2 &touch) const;
};

#endif
