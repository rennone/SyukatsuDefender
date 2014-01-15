#ifndef ICON_LIST_H
#define ICON_LIST_H

#include "Actor.h"
#include <math/Vector2.h>

class TextureRegion;
class IconList;
class SpriteBatcher;
class Building;

class Icon
{
  friend class IconList;
  const Building *sample; //建物表示用のサンプルモデル  
  const TextureRegion *image;
  const TextureRegion *description;
public:
  Icon(Building *sample, TextureRegion *image, TextureRegion *description);
  ~Icon();

};

class IconList : public Actor
{
    static constexpr int maxIcon = 20; //最大のアイコン数
    Icon *icons[maxIcon];
    int select;  //選択しているアイコン : -1 -> 何も選択していない
    int iconNum; //現在のアイコンの数
    float iconSize; //アイコンのサイズ
    SpriteBatcher *batcher;
public: 
    IconList(string name, SyukatsuGame *game);
    ~IconList();
    void addIcon(Icon *icon);
    void render(float deltaTime);
    bool selectIcon(const Vector2 &touch);
    int getSelectIcon() const;
    float getSelectIconRange() const;
};

#endif
