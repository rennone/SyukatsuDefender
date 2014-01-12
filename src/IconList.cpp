#include "IconList.h"
#include "Assets.h"
#include "PlayScene.h"
#include <syukatsu/syukatsu.h>

Icon::Icon(string _discription, TextureRegion *_image)
  :discription(_discription)
  ,image(_image)
{
}

Icon::~Icon()
{  
}

IconList::IconList(string name, SyukatsuGame *game)
  :Actor(name, game)
  ,select(-1)
  ,iconNum(0)
  ,iconSize(PlayScene::getMenuWindowWidth()/2)
{
  batcher = new SpriteBatcher(maxIcon*2);

  addIcon(new Icon("light", Assets::lightningTowerIcon));
  addIcon(new Icon("barrack", Assets::barrackIcon));
}

IconList::~IconList()
{
  for(int i=0; i<iconNum; i++)
    delete icons[i];
  
}

void IconList::addIcon(Icon *icon)
{
  if(iconNum >= maxIcon)
    return;
  
  icons[iconNum++] = icon;  
}

#include "Debugger.h"
void IconList::render(float deltaTime)
{
  glPushAttrib(GL_ENABLE_BIT | GL_COLOR_MATERIAL);

  const int left = -PlayScene::getMenuWindowWidth()/2;
  const int top  = +PlayScene::getMenuWindowHeight()/2;

  /*
  glEnable(GL_ALPHA_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
  glEnable(GL_BLEND);  
  */
  batcher->beginBatch(Assets::textureAtlas);

  for(int i=0; i<iconNum; i++)
  {
    int x = i%2;
    int y = i>>1;    
    batcher->drawSprite( left+(x+0.5)*iconSize, top-(y+0.5)*iconSize, iconSize, iconSize, icons[i]->image);

    if( i == select)
      batcher->drawSprite( left+(x+0.5)*iconSize, top-(y+0.5)*iconSize, iconSize, iconSize, Assets::highLight);
  }
  batcher->endBatch();
  glPopAttrib();  
}

bool IconList::selectIcon(const Vector2 &touch)
{  
  float x =  touch.x+PlayScene::getMenuWindowWidth()/2;
  float y = PlayScene::getMenuWindowHeight()/2-touch.y;
  int i = (x/iconSize);
  int j = (y/iconSize);
  int index = j*2 + i;
  
  if( index < 0 || iconNum <= index)      
    select = -1;
  else
    select = index;

  return select != -1; //選択しているかを返す  
}

int IconList::getSelectIcon() const
{
  return select;  
}




