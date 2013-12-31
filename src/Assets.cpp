#include "Assets.h"
#include <syukatsu/syukatsu.h>

Texture   *Assets::textureAtlas = NULL;
TextureRegion *Assets::virus = NULL;
TextureRegion *Assets::start = NULL;
TextureRegion *Assets::title = NULL;
TextureRegion *Assets::player = NULL;
TextureRegion *Assets::bullet = NULL;
TextureRegion *Assets::ground = NULL;
TextureRegion *Assets::frame = NULL;
TextureRegion *Assets::focusedFrame = NULL;
TextureRegion *Assets::background = NULL;
//Model    *Assets::simpleModel = NULL;
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
  int size = 64;
  virus = create(7,10, 2, 2);
  start = create(7, 1, 4, 1);
  title = create(7, 1, 4, 7);
  bullet = create(11,1,1,1);
  ground = create(0,4,4,4);
  frame  = create(9,2,1,1);
  focusedFrame  = create(8,2,1,1);
  background  = create(7,4,1,1);
  /*
  virus  = new TextureRegion(textureAtlas , 7*size, 10*size, 2*size, 2*size);
  start  = new TextureRegion(textureAtlas , 7*size,  1*size, 4*size, 1*size);
  title  = new TextureRegion(textureAtlas , 7*size,  1*size, 4*size, 7*size);
  player = new TextureRegion(textureAtlas, 10*size,10*size, 2*size, 2*size);
  bullet = new TextureRegion(textureAtlas, 11*size, 1*size, 1*size, 1*size);
  ground = new TextureRegion(textureAtlas, 0*size, 4*size, 4*size, 4*size);
  frame  = new TextureRegion(textureAtlas, 9*size, 2*size, 1*size, 1*size);
  focusedFrame  = new TextureRegion(textureAtlas, 8*size, 2*size, 1*size, 1*size);
  background  = new TextureRegion(textureAtlas, 7*size, 4*size, 1*size, 1*size);
  */
//  simpleModel = new XfileModel("sampleModel.x", 1);

  mincho = new SyukatsuFont("UtsukushiMincho.ttf");  
}

void Assets::dispose()
{
  delete textureAtlas;
  delete virus;
  //delete simpleModel;
  delete bullet;  
}

