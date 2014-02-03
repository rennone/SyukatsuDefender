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

  //いくつかのシーンで共通で使われる, それぞれのアトラスにある
  static TextureRegion *pressKey;
  static TextureRegion *highLight;
  static TextureRegion *numbers[10];
  
  static Texture   *titleAtlas;
  static TextureRegion *titleBackground;
  static TextureRegion *titleLogo;
    
  static Texture *selectAtlas;
  static TextureRegion *stageIcons[Information::STAGE_NUM];
  
  static Texture *playAtlas;
  
  static TextureRegion *buildPhase, *battlePhase;
  static TextureRegion *redRange, *greenRange;  
  static TextureRegion *buildingIcons[Information::BUILDING_NUM];
  static TextureRegion *buttonIcons[Information::BUTTON_NUM];
  static TextureRegion *regionFrame;
  
  static Texture *resultAtlas;
  static TextureRegion *victory;
  static TextureRegion *defeated;
  
  static Texture *fieldAtlas;
  static TextureRegion *mapChip[4];

  static Texture *skyboxAtlas;
  static TextureRegion *skybox[6];

  static TextureRegion *thunder[10];
  static SyukatsuFont     *mincho;
  static SyukatsuFont  *messageFont;
  
  static Model *buildings[Information::BUILDING_NUM];
  static Model *enemies[Information::ENEMY_NUM];  
  
  static void load();
  static void dispose();
private:
  Assets();
  Assets& operator=(const Assets& obj);
};

#endif
