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
  
  textureAtlas = new SyukatsuTexture("textureAtlas.png");
  int size = 64;  
  virus  = new TextureRegion(textureAtlas , 7*size, 10*size, 2*size, 2*size);
  start  = new TextureRegion(textureAtlas , 7*size,  1*size, 4*size, 1*size);
  title  = new TextureRegion(textureAtlas , 7*size,  1*size, 4*size, 7*size);
  player = new TextureRegion(textureAtlas, 10*size,10*size, 2*size, 2*size);
  bullet = new TextureRegion(textureAtlas, 11*size, 1*size, 1*size, 1*size);
  ground = new TextureRegion(textureAtlas, 0*size, 4*size, 4*size, 4*size);
  frame  = new TextureRegion(textureAtlas, 9*size, 2*size, 1*size, 1*size);
  focusedFrame  = new TextureRegion(textureAtlas, 8*size, 2*size, 1*size, 1*size);
  background  = new TextureRegion(textureAtlas, 7*size, 4*size, 1*size, 1*size);
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

