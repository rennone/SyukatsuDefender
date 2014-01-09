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
TextureRegion *Assets::mapChip[4];  
Model    *Assets::simpleModel = NULL;
Font     *Assets::mincho = NULL;

void Assets::load()
{
  //スタティック変数のパスを設定する為だけの捨てインスタンス
  FilePath path;
  path.setPath("../resource/");

  auto create = [](int l, int b, int w, int h)->TextureRegion*
  {
    int size = 64;    
    return new TextureRegion(Assets::textureAtlas , l*size, b*size, w*size, h*size);
  };
    
  textureAtlas = new SyukatsuTexture("textureAtlas.png");
  virus = create(7,10, 2, 2);
  start = create(7, 1, 4, 1);
  title = create(7, 1, 4, 7);
  bullet = create(11,1,1,1);
  frame  = create(9,2,1,1);
  focusedFrame  = create(8,2,1,1);
  background = create(7,4,1,1);
  mapChip[0] = create(0,4,4,4);
  mapChip[1] = create(0,0,1,1);  

  simpleModel = new XfileModel("sampleModel.x", 0.1);

  mincho = new SyukatsuFont("UtsukushiMincho.ttf");  
}

void Assets::dispose()
{
  delete textureAtlas;
  delete virus;
  //delete simpleModel;
  delete bullet;  
}

