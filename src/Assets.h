#ifndef ASSETS_H_2013_12_22
#define ASSETS_H_2013_12_22

#include <Texture.h>
#include <Model.h>
#include <Font.h>
#include <string>
#include <map>
#include "Information.h"
#include "BaseStatus.h"

using namespace std;

class SyukatsuFont;

class Assets
{
public:
  static string resourcePath;
  
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
  
  static Texture *resultAtlas;
  static TextureRegion *victory;
  static TextureRegion *defeated;
  
  static Texture *fieldAtlas;
  static TextureRegion *mapChip[4];
  static TextureRegion *buildable;
  static TextureRegion *unBuildable;
  static TextureRegion *regionFrame;
  
  static Texture *skyboxAtlas;
  static TextureRegion *skybox[6];

  static TextureRegion *thunder[10];
  static SyukatsuFont     *mincho;
  static SyukatsuFont  *messageFont;

  static Texture *frameAtlas;
  static TextureRegion *frameEdge[Information::FRAME_NUM];
  static TextureRegion *frameHorizontal[Information::FRAME_NUM];
  static TextureRegion *frameVertical[Information::FRAME_NUM]; //枠の外枠

   //枠の塗りつぶし用
  static TextureRegion *frameFillEdge[Information::FRAME_NUM];
  static TextureRegion *frameFillHorizontal[Information::FRAME_NUM];
  static TextureRegion *frameFillVertical[Information::FRAME_NUM];
  static TextureRegion *frameFillBackground;

  static Model *buildings[Information::BUILDING_NUM];
  static Model *barrack; //
  static Model *enemies[Information::ENEMY_NUM];
  static Model *fieldModel;
  
  static void load();
  static void dispose();

  static Texture *bitmapFont;
  static TextureRegion* bitmapChar['}'+10]; //mapでするよりも速そうだから, 配列で取っておく

  
  static BaseStatus *baseStatus;
private:
  Assets();
  Assets& operator=(const Assets& obj);
};

#endif
