#include <sstream>
#include <iomanip>
#include "MenuWindow.h"
#include "PlayScene.h"
#include "Assets.h"
#include "MessageManager.h"

MenuWindow::MenuWindow(string text, SyukatsuGame *game, Camera2D *_camera)
  :Actor(text, game)
  ,camera(_camera)
  ,select(-1)
{
  setStatus(Actor::UpdateOnly);
  
  //数が決まるまでは, 倍の要領取っておく
  batcher = new SpriteBatcher( (Information::BUILDING_NUM+Information::BUTTON_NUM) *2);

  const float left = -PlayScene::getMenuWindowWidth()/2;
  const float top  = +PlayScene::getMenuWindowHeight()/2;

  const float menuWidth  = PlayScene::getMenuWindowWidth();
  const float menuHeight  = PlayScene::getMenuWindowHeight();
  float iconSize = menuWidth/2;  
  float buttonY = top;
  for(int i=0; i<Information::BUILDING_NUM; i++)
  {
    int x = left + (i%2 )*iconSize;
    int y = top  - ((i>>1)+1)*iconSize;

    towerIcons[i] = new Icon(Vector2( x, y),
                        Vector2( iconSize, iconSize),
                        Assets::buildingIcons[i]);
    buttonY = towerIcons[i]->lowerLeft.y;
  }
  
  float buttonHeight = menuWidth/3.0;    //delete, upgradeButtonの横幅

  buttonY += buttonHeight/20;
  for(int i=0; i<Information::BUTTON_NUM; i++)
  {
    buttonY -= buttonHeight;
    buttons[i] = new Icon(Vector2( left, buttonY),
                          Vector2( menuWidth, buttonHeight),
                          Assets::buttonIcons[i]);
  }
}

MenuWindow::~MenuWindow()
{
  delete batcher;
  for(int i=0; i<Information::BUILDING_NUM; i++)
    delete towerIcons[i];

  for(int i=0; i<Information::BUTTON_NUM; i++)
    delete buttons[i];
}

void MenuWindow::reshape(int width, int height)
{
  const float left = -PlayScene::getMenuWindowWidth()/2;
  const float top  = +PlayScene::getMenuWindowHeight()/2;

  const float menuWidth  = PlayScene::getMenuWindowWidth();

  float iconSize = menuWidth/2;
  float buttonY = top;
  
  for(int i=0; i<Information::BUILDING_NUM; i++)
  {
    int x = i%2;
    int y = i>>1;

    towerIcons[i]->lowerLeft.set( left+x*iconSize, top-(y+1)*iconSize);
    towerIcons[i]->size.set( iconSize, iconSize);
    
    buttonY = towerIcons[i]->lowerLeft.y;
  }
  
  float buttonHeight     = menuWidth/3.0;    //delete, upgradeButtonの横幅

  for(int i=0; i<Information::BUTTON_NUM; i++)
  {
    buttonY -= buttonHeight;
    buttons[i]->lowerLeft.set( left, buttonY);
    buttons[i]->size.set(menuWidth, buttonHeight);
  }  
}

void MenuWindow::update(float deltaTime)
{
  //ボタンをプッシュしたかは, 毎フレーム初期化する
  push = -1;
  auto mouseEvent = syukatsuGame->getInput()->getMouseEvent();
  
  if ( mouseEvent->action == GLFW_PRESS )
  {
    Vector2 point(mouseEvent->x, mouseEvent->y);
    auto touch = camera->screenToWorld(point);

    setSelectedIcon(touch);
    setPushedButton(touch);
  }

  auto keyEvents = syukatsuGame->getInput()->getKeyEvents();
  
  for (auto event : keyEvents)
  {
    if(event->action != GLFW_PRESS)
      continue;

    //アイコンを選択しているか
    for ( int i=0; i<Information::BUILDING_NUM; i++ )
    {
      if ( event->keyCode != Information::BuildingShortCutKeys[i] )
        continue;

      select = i;
      break;  //同時押しの時に速く押した方が優先されるように
    }
    
    //ボタンを選択しているか
    for ( int i=0; i<Information::BUTTON_NUM; i++)
    {
      if ( event->keyCode != Information::ButtonShortCutKeys[i] )
        continue;

      push = i;
      break;
    }
  }
}

void MenuWindow::render(float deltaTime)
{  
  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);  
  glDisable(GL_DEPTH_TEST);  //これがあると2Dでは, 透過画像が使えないので消す
  glDisable(GL_LIGHTING);

  camera->setViewportAndMatrices();

  const float width = PlayScene::getMenuWindowWidth();
  const float height = PlayScene::getMenuWindowHeight();
  const float left = -width/2;
  const float top  = +height/2;
  const float lineWidth = 1.0;

  //メニューの外枠表示
  MessageManager::drawFrame( Information::SOLID, Vector2(left+lineWidth/2, top-lineWidth/2), Vector2(width-lineWidth, height-lineWidth), lineWidth, TextColors::BlackText);
  for(int i=0; i<4; i++)
  {
    const Vector2 upperLeft( left+(i&1)*width/2+(1-(i&1))*lineWidth/2,
                             top-(i>>1)*width/2-(1-(i>>1))*lineWidth/2);
    const Vector2 size(width/2-lineWidth/2, width/2-lineWidth/2);
    MessageManager::drawFrame( Information::SOLID, upperLeft, size, lineWidth, TextColors::BlackText);
  }  
  
  batcher->beginBatch(Assets::playAtlas);
  for (auto icon : towerIcons)
  {
    batcher->drawSprite( icon->lowerLeft.x+icon->size.x/2,
                         icon->lowerLeft.y+icon->size.y/2,
                         icon->size.x, icon->size.y,  icon->image);
  }

  for( auto button : buttons)
  {
    batcher->drawSprite( button->lowerLeft.x+button->size.x/2,
                         button->lowerLeft.y+button->size.y/2,
                         button->size.x, button->size.y, button->image);

    const Vector2 upperLeft( button->lowerLeft.x, button->lowerLeft.y + button->size.y+lineWidth/2);
    const Vector2 size(width, button->size.y);
    MessageManager::drawFrame( Information::SOLID, upperLeft, size, lineWidth, TextColors::BlackText);
  }
  
  //選択しているアイコンのハイライトと説明文表示
  if(select != -1)
  {
      //ハイライト
    batcher->drawSprite( towerIcons[select]->lowerLeft.x + towerIcons[select]->size.x/2,
                         towerIcons[select]->lowerLeft.y + towerIcons[select]->size.y/2,
                         towerIcons[select]->size.x*0.8,  towerIcons[select]->size.y*0.8,
                         Assets::highLight);
    
    //説明文
    const float menuWidth = PlayScene::getMenuWindowWidth();
    const float charSize = PlayScene::getMenuWindowWidth()/11.0;
    std::stringstream ss;
    auto baseStatus = Assets::baseStatus->getBuildingBaseStatus(select);
    ss << Information::BuildingName[select] << std::endl << std::endl;
    ss << Information::DescriptionOfBuildings[select] << std::endl << std::endl;
    ss << "price   $" << baseStatus->getBaseValue() << std::endl;
    ss << "damage  " << baseStatus->getAttack() << std::endl;
    ss << "rate    "  << std::setprecision(2) << 1.0 / baseStatus->getAttackRate();
    MessageManager::drawBitmapString(ss.str(),
                                     Vector2(-PlayScene::getMenuWindowWidth()/2+charSize/2, 0 - 4*charSize), charSize);    
  }
  batcher->endBatch();

  glPopAttrib();
}

void MenuWindow::drawBuildingInfo(Building* building)
{
    //説明文
    const float menuWidth = PlayScene::getMenuWindowWidth();

    const float charSize = PlayScene::getMenuWindowWidth()/11.0;
    std::stringstream ss;
    auto baseStatus = Assets::baseStatus->getBuildingBaseStatus(select);
    ss << building->getName() << std::endl << std::endl;
    ss << building->getDescription() << std::endl << std::endl;
    ss << "level   " << building->getLevel() << "/" << building->getMaxLevel() << std::endl;
    ss << "damage  " << building->calcAttack() << std::endl;
    ss << "rate    "  << std::setprecision(2) << 1.0 / building->calcAttackRate() << std::endl;
    ss << "upgrade $" << building->getUpgradeCost();
    MessageManager::drawBitmapString(ss.str(), Vector2(-PlayScene::getMenuWindowWidth()/2+charSize/2, 0-4*charSize), charSize);
}


void MenuWindow::setSelectedIcon(const Vector2 &touch)
{
  select = -1;
  for(int i=0; i<Information::BUILDING_NUM; i++)
  {
    if( !towerIcons[i]->inRegion(touch) )
      continue;
    
    select = i;
    return;
  }
}

void MenuWindow::setPushedButton(const Vector2 &touch)
{
  for(int i=0; i<Information::BUTTON_NUM; i++)
  {
    if( !buttons[i]->inRegion(touch) )
      continue;
    
    push = i;
    return;
  }
}
