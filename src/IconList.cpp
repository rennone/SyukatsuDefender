#include "IconList.h"
#include "Assets.h"
#include "PlayScene.h"
#include "Information.h"
#include "LightningTower.h"
#include "Barrack.h"

#include <syukatsu/syukatsu.h>

Icon::Icon(Building *_sample, TextureRegion *_image, TextureRegion *_description)
  :sample(_sample)
  ,image(_image)
  ,description(_description)
{
}

Icon::~Icon()
{
delete sample;
}

IconList::IconList(string name, SyukatsuGame *game)
  :Actor(name, game)
  ,select(-1)
  ,iconNum(0)
  ,iconSize(PlayScene::getMenuWindowWidth()/2)
{
  batcher = new SpriteBatcher(maxIcon*2);

  for(int i=0; i<Information::BUILDING_NUM; i++)
  {
    icons[i] = new Icon(new LightningTower("building", NULL, NULL, NULL),
                        Assets::buildingIcons[i],
                        Assets::buildingIcons[i]);
    iconNum++;
  }  
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

  const float left = -PlayScene::getMenuWindowWidth()/2;
  const float top  = +PlayScene::getMenuWindowHeight()/2;

  
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

  if(select != -1)
  {
    const float menuWidth = PlayScene::getMenuWindowWidth();
    const float menuHeight = menuWidth*3.0/4.0;
    batcher->drawSprite( 0, -top+menuHeight/2, menuWidth, menuHeight, icons[select]->description);
  }
  batcher->endBatch();
  glPopAttrib();  
}

bool IconList::selectIcon(const Vector2 &touch)
{
  float x = touch.x+PlayScene::getMenuWindowWidth()/2;
  float y = PlayScene::getMenuWindowHeight()/2-touch.y;

  if(x < 0 || y < 0 || x > PlayScene::getMenuWindowWidth() || y > PlayScene::getMenuWindowHeight())
  {
    select = -1;
    return false;    
  }
  
  int i = floor(x/iconSize);
  int j = floor(y/iconSize);

  int index = j*2 + i;  
  if(i < 0 || j < 0 || index < 0 || iconNum <= index)      
    select = -1;
  else
    select = index;

  return select != -1; //選択しているかを返す  
}

bool IconList::selectIcon(const int type)
{
  if(iconNum <= type) {
    select = -1;
  }
  else {
    select = type;
  }

  return select != -1;
}

int IconList::getSelectIcon() const
{
  return select;  
}

float IconList::getSelectIconRange() const
{
if(select<0 || select >= iconNum)
  return -1;

return icons[select]->sample->getRangeOfEffect();
}




