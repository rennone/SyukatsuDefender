#include "Assets.h"
#include <syukatsu/syukatsu.h>

Texture   *Assets::textureAtlas = NULL;
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

TextureRegion *Assets::buildPhase = NULL;
TextureRegion *Assets::battlePhase = NULL;
TextureRegion *Assets::mapChip[4];
TextureRegion *Assets::highLight;  
TextureRegion *Assets::greenRange;
TextureRegion *Assets::redRange;
TextureRegion *Assets::regionFrame;
SyukatsuFont     *Assets::mincho = NULL;
SyukatsuFont     *Assets::messageFont = NULL;
Model *Assets::buildings[Information::BUILDING_NUM];
TextureRegion *Assets::buildingIcons[Information::BUILDING_NUM];
TextureRegion *Assets::buttonIcons[Information::BUTTON_NUM];
TextureRegion *Assets::stageIcons[Information::STAGE_NUM];

Model *Assets::enemies[Information::ENEMY_NUM];

TextureRegion *Assets::victory;
TextureRegion *Assets::defeated;

void Assets::load()
{
  //スタティック変数のパスを設定する為だけの捨てインスタンス
  FilePath path;
  path.setPath("../resource/");


  auto create = [](Texture* texture, int l, int b, int w, int h)->TextureRegion*
  {
    int size = 64;    
    return new TextureRegion(texture , l*size+1, b*size+1, w*size-2, h*size-2);
  };
    
  textureAtlas = new SyukatsuTexture("textureAtlas.png");


  
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
    
  fieldAtlas = new SyukatsuTexture("fieldAtlas.png");
  skyboxAtlas = new SyukatsuTexture("fieldAtlas.png");
  
  mapChip[0] = create(fieldAtlas, 0,0,1,1);
  mapChip[1] = create(fieldAtlas, 5,0,1,1);
  regionFrame = create(fieldAtlas, 1,12,1,1);
  redRange  = create(playAtlas, 1,2,1,1);
  greenRange= create(playAtlas, 0,2,1,1);

  for(int i=0; i<6; i++)
    skybox[i] = create(skyboxAtlas, 0, 12, 1, 1);
  
  buildings[Information::LIGHTNING_TOWER] = new XfileModel("lightningTower.x", 0.1);
  buildings[Information::FREEZING_TOWER]  = new XfileModel("lightningTower.x", 0.1);
  buildings[Information::ARROW_TOWER]     = new XfileModel("lightningTower.x", 0.1); 
  buildings[Information::BARRACK]         = new XfileModel("barrack.x", 0.1);

  buildingIcons[Information::LIGHTNING_TOWER] = create(playAtlas, 0,1, 1,1);
  buildingIcons[Information::FREEZING_TOWER]  = create(playAtlas, 2,1, 1,1);
  buildingIcons[Information::ARROW_TOWER]     = create(playAtlas, 3,1, 1,1);

  buttonIcons[Information::DELETE_BUTTON]  = create(playAtlas, 0, 5, 3, 1);
  buttonIcons[Information::UPGRADE_BUTTON] = create(playAtlas, 3, 5, 3, 1);

  enemies[Information::SOLDIER0] = new XfileModel("soldier0.x", 0.05);
  enemies[Information::SOLDIER1] = new XfileModel("soldier1.x", 0.05);
  enemies[Information::SOLDIER2] = new XfileModel("soldier2.x", 0.05);
  enemies[Information::SOLDIER3] = new XfileModel("soldier3.x", 0.05);
  enemies[Information::SOLDIER4] = new XfileModel("soldier4.x", 0.05);



  resultAtlas = new SyukatsuTexture("resultAtlas.png");
  victory  = create(resultAtlas, 0, 1, 4, 1);
  defeated = create(resultAtlas, 0, 2, 4, 1);

  mincho      = new SyukatsuFont("UtsukushiMincho.ttf");
  mincho->setSize(5);
  
  messageFont = new SyukatsuFont("UtsukushiMincho.ttf");
  messageFont->setSize(24);  
}

void Assets::dispose()
{
  delete textureAtlas;
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

