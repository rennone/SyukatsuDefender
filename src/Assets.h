#ifndef ASSETS_H_2013_12_22
#define ASSETS_H_2013_12_22

#include <Texture.h>
#include <Model.h>
#include <Font.h>
#include <string>
#include "Information.h"

using namespace std;

class SyukatsuFont;

class Assets
{
public:
  static string resourcePath;
  static Texture   *textureAtlas;
  static TextureRegion *virus;
  static TextureRegion *player;
  static TextureRegion *start;
  static TextureRegion *title;
  static TextureRegion *bullet;
  static TextureRegion *frame;
  static TextureRegion *lightningTowerIcon;
  static TextureRegion *barrackIcon;
  static TextureRegion *focusedFrame;
  static TextureRegion *highLight;
  static TextureRegion *range;  
  static TextureRegion *background;
  static TextureRegion *mapChip[4];
  static TextureRegion *explosion;  
  static Model    *simpleModel;
  static SyukatsuFont     *mincho;
  static SyukatsuFont  *messageFont;  
  static Model *buildings[Information::BUILDING_NUM];
  static TextureRegion *buildingIcons[Information::BUILDING_NUM];
  static TextureRegion *buttonIcons[Information::BUTTON_NUM];
  
  static TextureRegion *victory;
  static TextureRegion *defeated;
  static void load();
  static void dispose();
private:
  Assets();
  Assets& operator=(const Assets& obj);
};

#endif
