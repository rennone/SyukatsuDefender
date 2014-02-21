#include "Assets.h"
#include <syukatsu/syukatsu.h>
#include <stdio.h>

Texture   *Assets::titleAtlas = NULL;
Texture   *Assets::selectAtlas = NULL;
Texture   *Assets::playAtlas = NULL;
Texture   *Assets::resultAtlas = NULL;
Texture   *Assets::fieldAtlas = NULL;
Texture   *Assets::skyboxAtlas;
TextureRegion *Assets::skybox[6];
TextureRegion *Assets::pressKey = NULL;
TextureRegion *Assets::titleLogo = NULL;
TextureRegion *Assets::titleBackground = NULL;
TextureRegion *Assets::numbers[10];
TextureRegion *Assets::buildable;
TextureRegion *Assets::unBuildable;

TextureRegion *Assets::thunder[10];
TextureRegion *Assets::buildPhase = NULL;
TextureRegion *Assets::battlePhase = NULL;
TextureRegion *Assets::mapChip[4];
TextureRegion *Assets::highLight;  
TextureRegion *Assets::greenRange;
TextureRegion *Assets::redRange;
TextureRegion *Assets::regionFrame;
SyukatsuFont  *Assets::mincho = NULL;
SyukatsuFont  *Assets::messageFont = NULL;
Model *Assets::buildings[Information::BUILDING_NUM];
TextureRegion *Assets::buildingIcons[Information::BUILDING_NUM];
TextureRegion *Assets::buttonIcons[Information::BUTTON_NUM];
TextureRegion *Assets::stageIcons[Information::STAGE_NUM];

Model *Assets::enemies[Information::ENEMY_NUM];
Model *Assets::fieldModel = NULL;
Model *Assets::barrack = NULL;
TextureRegion *Assets::victory;
TextureRegion *Assets::defeated;

Texture *Assets::bitmapFont;
TextureRegion *Assets::bitmapChar['}'+10];

//枠の端, 横棒, 縦棒
TextureRegion *Assets::frameEdge;
TextureRegion *Assets::frameHorizontal;
TextureRegion *Assets::frameVertical;

BaseStatus *Assets::baseStatus;

void Assets::load()
{
  //スタティック変数のパスを設定する為だけの捨てインスタンス
  FilePath path;
  path.setPath("../resource/");

  auto create = [](Texture* texture, float l, float b, float w, float h)->TextureRegion*
  {
    int size = 64; 
    return new TextureRegion(texture , l*size+2, b*size+2, w*size-4, h*size-4);
  };
    
//  textureAtlas = new SyukatsuTexture("textureAtlas.png");
  
  titleAtlas = new SyukatsuTexture("titleAtlas.png"); //タイトル用テクスチャアトラス
  pressKey  = create(titleAtlas, 0, 0, 5, 1); //プレスキー メッセージ(他のシーンでも共通)
  highLight = create(titleAtlas, 5, 0, 1, 1);
  
  titleBackground = create(titleAtlas, 0, 0, 5, 1);
  titleLogo       = create(titleAtlas, 0, 1, 8, 2);

  
  selectAtlas = new SyukatsuTexture("selectAtlas.png");
  stageIcons[Information::STAGE1] = create(selectAtlas, 0, 1, 1, 1);
  stageIcons[Information::STAGE2] = create(selectAtlas, 1, 1, 1, 1);
  stageIcons[Information::STAGE3] = create(selectAtlas, 2, 1, 1, 1);
  
  playAtlas = new SyukatsuTexture("playAtlas.png");
  battlePhase = create(playAtlas, 8, 4, 8, 2);
  buildPhase  = create(playAtlas, 8, 6, 8, 2);
  for(int i=0; i<10; i++)
    numbers[i] = create(playAtlas, i+6, 0, 1, 1);

  for(int i=0; i<10; i++)
  {
    thunder[i] = create(playAtlas, 7,4, 1.0, 4.0*(i+1)/10.0);
  }
    
  fieldAtlas = new SyukatsuTexture("fieldAtlas.png");
  skyboxAtlas = new SyukatsuTexture("fieldAtlas.png");
  
  mapChip[Information::Bush] = create(fieldAtlas, 0,0,1,1);
  mapChip[Information::Road] = create(fieldAtlas, 5,0,1,1);

  regionFrame = create(fieldAtlas, 1,12,1,1);
  buildable   = create(fieldAtlas, 2,12,1,1);
  unBuildable = create(fieldAtlas, 1,12,1,1);
  redRange  = create(playAtlas, 1,2,1,1);
  greenRange= create(playAtlas, 0,2,1,1);

  for(int i=0; i<6; i++)
    skybox[i] = create(skyboxAtlas, 0, 12, 1, 1);
  
  buildings[Information::LIGHTNING_TOWER] = new XfileModel("lightningTower.x", 0.1);
  buildings[Information::FREEZING_TOWER]  = new XfileModel("FreezingTower.x", 0.3);
  buildings[Information::ARROW_TOWER]     = new XfileModel("ArrowTower.x", 0.25); 
  barrack         = new XfileModel("barrack.x", 0.1);

  buildingIcons[Information::LIGHTNING_TOWER] = create(playAtlas, 0,1, 1,1);
  buildingIcons[Information::FREEZING_TOWER]  = create(playAtlas, 2,1, 1,1);
  buildingIcons[Information::ARROW_TOWER]     = create(playAtlas, 3,1, 1,1);

  buttonIcons[Information::DELETE_BUTTON]  = create(playAtlas, 0, 5, 3, 1);
  buttonIcons[Information::UPGRADE_BUTTON] = create(playAtlas, 3, 5, 3, 1);

  enemies[Information::SOLDIER3] = new XfileModel("do.x", 0.3);
  enemies[Information::SOLDIER] = new XfileModel("Griffin.x", 0.15);
  enemies[Information::KNIGHT] = new XfileModel("gargoyle.x", 0.2);
  enemies[Information::HEAVYARMOR] = new XfileModel("minotaurus.x", 0.1);
  enemies[Information::SOLDIER4] = new XfileModel("dragon.x", 0.3);

  fieldModel = new XfileModel("field.x", 0.5);

  resultAtlas = new SyukatsuTexture("resultAtlas.png");
  victory  = create(resultAtlas, 0, 1, 4, 1);
  defeated = create(resultAtlas, 0, 2, 4, 1);

  frameEdge       = create(playAtlas, 8,1, 1,1);
  frameHorizontal = create(playAtlas, 9,1, 1,1);
  frameVertical   = create(playAtlas, 8,2, 1,1);

  mincho      = new SyukatsuFont("UtsukushiMincho.ttf");
  mincho->setSize(5);
  
  messageFont = new SyukatsuFont("UtsukushiMincho.ttf");
  messageFont->setSize(24);

  bitmapFont = new SyukatsuTexture("bitmapFont.png");
  auto charCreate = [](Texture* texture, float l, float b, float w, float h)->TextureRegion*
  {
    float size = 512.0/10.0; 
    return new TextureRegion(texture , l*size+2, b*size+2, w*size-4, h*size-4);
  };

  int numChar = '}';
  for(int i = ' ', j = 0; i <= numChar; i++)
  {
    bitmapChar[i] = charCreate(bitmapFont, j % 10, j / 10, 1, 1);
    j++;
  }

  baseStatus = new BaseStatus();
}

void Assets::dispose()
{
  delete titleAtlas;
  delete selectAtlas;
  delete playAtlas;  
  delete resultAtlas;

  delete pressKey;
  delete titleLogo;
  delete titleBackground;
  
  for(int i=0; i<Information::STAGE_NUM; i++)
    delete stageIcons[i];

  for(int i=0; i<Information::BUILDING_NUM; i++)
  {
    delete buildings[i];    
  }  
}

