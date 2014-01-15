#include "Assets.h"
#include <syukatsu/syukatsu.h>

Texture   *Assets::textureAtlas = NULL;
TextureRegion *Assets::virus = NULL;
TextureRegion *Assets::start = NULL;
TextureRegion *Assets::title = NULL;
TextureRegion *Assets::player = NULL;
TextureRegion *Assets::bullet = NULL;
TextureRegion *Assets::frame = NULL;
TextureRegion *Assets::focusedFrame = NULL;
TextureRegion *Assets::background = NULL;
TextureRegion *Assets::explosion = NULL;
TextureRegion *Assets::lightningTowerIcon = NULL;
TextureRegion *Assets::barrackIcon = NULL;
TextureRegion *Assets::mapChip[4];
TextureRegion *Assets::highLight;  
Model    *Assets::simpleModel = NULL;
Font     *Assets::mincho = NULL;

Model *Assets::buildings[Information::BuildingNum];
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
  start = create(7, 1, 4, 1);
  title = create(7, 1, 4, 7);
  bullet = create(11,1,1,1);
  frame  = create(9,2,1,1);
  focusedFrame  = create(8,2,1,1);
  background = create(7,4,1,1);
  mapChip[0] = create(0,4,1,1);
  mapChip[1] = create(0,0,1,1);
  explosion = create(0, 3, 1, 1);
  lightningTowerIcon = create(0,1, 1,1);
  barrackIcon = create(1,1,1,1);
  highLight = create(2,1,1,1);  
  simpleModel = new XfileModel("sampleModel.x", 0.1);
  buildings[Information::LIGHTNING_TOWER] = new XfileModel("lightningTower.x", 0.1);
  buildings[Information::FREEZING_TOWER] = new XfileModel("lightningTower.x", 0.1);
  buildings[Information::BARRACK] = new XfileModel("barrack.x", 0.1);

  mincho = new SyukatsuFont("UtsukushiMincho.ttf");  
}

void Assets::dispose()
{
  delete textureAtlas;
  delete virus;
  //delete simpleModel;
  delete bullet;  
}

