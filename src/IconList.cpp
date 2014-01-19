#include "IconList.h"
#include "Assets.h"
#include "PlayScene.h"
#include "Information.h"
#include "LightningTower.h"
#include "Barrack.h"
#include <syukatsu/syukatsu.h>


using namespace Information;

Icon::Icon(const Vector2 &_lowerLeft, const Vector2 &_size, TextureRegion *_image)
  :lowerLeft(_lowerLeft)
  ,size(_size)
  ,image(_image)
{
}

Icon::~Icon()
{
  
}

inline bool Icon::inRegion(const Vector2 &touch) const
{
  return lowerLeft.x < touch.x && touch.x < lowerLeft.x+size.x && lowerLeft.y < touch.y && touch.y < lowerLeft.y + size.y;
}

IconList::IconList(string name, SyukatsuGame *game)
  :Actor(name, game)
  ,select(-1)
  ,iconNum(0)
  ,iconSize(PlayScene::getMenuWindowWidth()/2)
{
  //数が決まるまでは, 倍の要領取っておく
  batcher = new SpriteBatcher( (Information::BUILDING_NUM+Information::BUTTON_NUM) *2);

  const float left = -PlayScene::getMenuWindowWidth()/2;
  const float top  = +PlayScene::getMenuWindowHeight()/2;

  float lastY = top;
  for(int i=0; i<Information::BUILDING_NUM; i++)
  {
    int x = i%2;
    int y = i>>1;

    icons[i] = new Icon(Vector2( left+x*iconSize, top-(y+1)*iconSize),
                        Vector2( iconSize, iconSize),
                        Assets::buildingIcons[i]);
    iconNum++;
    lastY = icons[i]->lowerLeft.y;
  }

//  buildings[Information::LIGHTNING_TOWER] = new LightningTower();
  
  const float menuWidth  = PlayScene::getMenuWindowWidth();
  const float menuHeight = menuWidth*3.0/4.0;
  float buttonHeight     = menuWidth/3.0;    //delete, upgradeButtonの横幅

  for(int i=0; i<Information::BUTTON_NUM; i++)
  {
    lastY -= buttonHeight;
    buttons[i] = new Icon(Vector2( left, lastY),
                          Vector2( menuWidth, buttonHeight),
                          Assets::buttonIcons[i]);
  }
}

IconList::~IconList()
{
  for(int i=0; i<iconNum; i++)
    delete icons[i];  
}

void IconList::render(float deltaTime)
{
  glPushAttrib(GL_ENABLE_BIT | GL_COLOR_MATERIAL);

  const float left = -PlayScene::getMenuWindowWidth()/2;
  const float top  = +PlayScene::getMenuWindowHeight()/2;

  batcher->beginBatch(Assets::textureAtlas);
  for(int i=0; i<iconNum; i++)
  {
    float cx = icons[i]->lowerLeft.x+icons[i]->size.x/2;
    float cy = icons[i]->lowerLeft.y+icons[i]->size.y/2;
    batcher->drawSprite( cx, cy, icons[i]->size.x, icons[i]->size.y, icons[i]->image);
    
    //選択中のハイライト表示
    if( i == select)
      batcher->drawSprite( cx, cy, icons[i]->size.x, icons[i]->size.y, Assets::highLight);
  }

  const float menuWidth = PlayScene::getMenuWindowWidth();
  const float menuHeight = menuWidth*3.0/4.0;

  for(int i=0; i<2; i++)
  {
    batcher->drawSprite( buttons[i]->lowerLeft.x+buttons[i]->size.x/2,
                         buttons[i]->lowerLeft.y+buttons[i]->size.y/2,
                         buttons[i]->size.x,
                         buttons[i]->size.y,                         
                         buttons[i]->image);
  }
  //選択しているアイコンの説明文表示
  if(select != -1)    
    batcher->drawSprite( 0, -top+menuHeight/2, menuWidth, menuHeight, icons[select]->image);  
  
  batcher->endBatch();
  glPopAttrib();  
}

bool IconList::selectIcon(const Vector2 &touch)
{
  /*
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
  */
  select = -1;
  for(int i=0; i<iconNum; i++)
  {
    if( !icons[i]->inRegion(touch) )
      continue;
    
    select = i;
    return true;
  }
  return false; //選択しているかを返す  
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
  return 70;//icons[select]->sample->getRangeOfEffect();
}

int IconList::getTouchedButton(const Vector2 &touch) const
{
  for(int i=0; i<Information::BUTTON_NUM; i++)
  {
    if( !buttons[i]->inRegion(touch) )
      continue;
    
    return i;
  }

  return -1;
}



