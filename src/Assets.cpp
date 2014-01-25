#include "Assets.h"
#include <syukatsu/syukatsu.h>

Texture   *Assets::textureAtlas = NULL;
TextureRegion *Assets::virus = NULL;
TextureRegion *Assets::start = NULL;
TextureRegion *Assets::title = NULL;
TextureRegion *Assets::player = NULL;
TextureRegion *Assets::bullet = NULL;

TextureRegion *Assets::background = NULL;
TextureRegion *Assets::explosion = NULL;
TextureRegion *Assets::backHome = NULL;

TextureRegion *Assets::mapChip[4];
TextureRegion *Assets::highLight;  
TextureRegion *Assets::range;
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

  auto create = [](int l, int b, int w, int h)->TextureRegion*
  {
    int size = 64;    
    return new TextureRegion(Assets::textureAtlas , l*size+1, b*size+1, w*size-2, h*size-2);
  };
    
  textureAtlas = new SyukatsuTexture("textureAtlas.png");
  virus = create(7,10, 2, 2);
  start = create(7, 6, 7, 1);
  title = create(7, 7, 7, 1);
  bullet = create(11,1,1,1);

  background = create(7,4,1,1);
  mapChip[0] = create(0,4,1,1);
  mapChip[1] = create(0,0,1,1);
  explosion = create(0, 3, 1, 1);
  
  buildings[Information::LIGHTNING_TOWER] = new XfileModel("lightningTower.x", 0.1);
  buildings[Information::FREEZING_TOWER]  = new XfileModel("lightningTower.x", 0.1);
  buildings[Information::ARROW_TOWER]     = new XfileModel("lightningTower.x", 0.1); 
  buildings[Information::BARRACK]         = new XfileModel("barrack.x", 0.1);

  buildingIcons[Information::LIGHTNING_TOWER] = create(0,1, 1,1);
  buildingIcons[Information::FREEZING_TOWER]  = create(2,1, 1,1);
  buildingIcons[Information::ARROW_TOWER]     = create(3,1, 1,1);

  buttonIcons[Information::DELETE_BUTTON]  = create(0, 5, 3, 1);
  buttonIcons[Information::UPGRADE_BUTTON] = create(3, 5, 3, 1);

  enemies[Information::SOLDIER0] = new XfileModel("soldier0.x", 0.05);
  enemies[Information::SOLDIER1] = new XfileModel("soldier1.x", 0.05);
  enemies[Information::SOLDIER2] = new XfileModel("soldier2.x", 0.05);
  enemies[Information::SOLDIER3] = new XfileModel("soldier3.x", 0.05);
  enemies[Information::SOLDIER4] = new XfileModel("soldier4.x", 0.05);

  stageIcons[Information::STAGE1] = create(10, 1, 1, 1);
  stageIcons[Information::STAGE2] = create(11, 1, 1, 1);
  stageIcons[Information::STAGE3] = create(12, 1, 1, 1);
  
  highLight = create(0,2,1,1);
  range     = create(0,3,1,1);

  victory  = create(0, 7, 3, 1);
  defeated = create(3, 7, 3, 1);
  backHome = create(7, 6, 7, 1);
  mincho      = new SyukatsuFont("UtsukushiMincho.ttf");
  messageFont = new SyukatsuFont("UtsukushiMincho.ttf");
  messageFont->setSize(24);  
}

void Assets::dispose()
{
  delete textureAtlas;
  delete virus;
  //delete simpleModel;
  delete bullet;  
}

