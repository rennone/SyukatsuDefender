#include "StageSelectScene.h"
#include "Field.h"
#include "Information.h"
#include "Assets.h"
#include "PlayScene.h"
#include "Debugger.h"
#include "MessageManager.h"

StageSelectScene::StageSelectScene(SyukatsuGame *game)
  :SyukatsuScene(game)
  ,select(0)
{
  camera = new Camera3D(syukatsuGame->getWindow(), 1, 3000, 80);
  menuCamera = new Camera2D(syukatsuGame->getWindow(), MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
  titleCamera = new Camera2D(syukatsuGame->getWindow(), TITLE_WINDOW_WIDTH, TITLE_WINDOW_HEIGHT);
  
  int width, height;
  glfwGetFramebufferSize(syukatsuGame->getWindow(), &width, &height);
  cameraViewportSetting(width, height);
  
  field = new Field("sampleField", syukatsuGame);
  field->setLane(select);
  
  batcher = new SpriteBatcher(STAGE_NUM*2);  
  float iconSize = MENU_WINDOW_WIDTH/(float)(STAGE_NUM*2+1);  
  float bottom = -iconSize/2; //- MENU_WINDOW_HEIGHT/2;
  for(int i=0; i<Information::BUILDING_NUM; i++)
  {
    float left = -MENU_WINDOW_WIDTH/2 + (2*i+1)*iconSize;
    icons[i] = new Icon(Vector2(left, bottom), Vector2( iconSize, iconSize), Assets::stageIcons[i]);
  }
  
  glEnable(GL_TEXTURE_2D);
}

StageSelectScene::~StageSelectScene()
{
  delete batcher;
  delete field;
  delete camera;
  delete menuCamera;

  for(auto icon : icons)
    delete icon;
}

void StageSelectScene::reshape(int width, int height)
{
  cameraViewportSetting(width, height);
}

void StageSelectScene::cameraViewportSetting(int width, int height)
{
  float playWindowWidth  = width/3;
  float playWindowHeight = playWindowWidth;

  camera->setViewportWidth(playWindowWidth);
  camera->setViewportHeight(playWindowHeight);
  camera->setViewportPosition(width/2, height/2);

  camera->setLook(Vector3(Field::cellNum*Field::cellSize/2, 0, Field::cellNum*Field::cellSize/2));
  camera->setPosition(Vector3(0, Field::cellNum*Field::cellSize, 0));

  //2DWindowは画面いっぱいにする(タイトルとアイコン両方表示させるため)
  float menuWindowWidth  = width;
  float menuWindowHeight = height/4.0;
  float menuRatio = menuWindowWidth/(float)menuWindowHeight;
  
  MENU_WINDOW_HEIGHT = 100.0f;
  MENU_WINDOW_WIDTH  = MENU_WINDOW_HEIGHT*menuRatio;

  menuCamera->setFrustumSize(Vector2(MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT));
  menuCamera->setViewportWidth(menuWindowWidth);
  menuCamera->setViewportHeight(menuWindowHeight);
  menuCamera->setViewportPosition(menuWindowWidth/2, menuWindowHeight/2);

  float titleWindowWidth  = width;
  float titleWindowHeight = height/4.0;
  float titleRatio = titleWindowWidth/(float)titleWindowHeight;
  TITLE_WINDOW_HEIGHT = 100.0f;
  TITLE_WINDOW_WIDTH  = TITLE_WINDOW_HEIGHT*titleRatio;

  titleCamera->setFrustumSize(Vector2(TITLE_WINDOW_WIDTH, TITLE_WINDOW_HEIGHT));
  titleCamera->setViewportWidth(titleWindowWidth);
  titleCamera->setViewportHeight(titleWindowHeight);
  titleCamera->setViewportPosition(titleWindowWidth/2, height-titleWindowHeight/2);
}

void StageSelectScene::update(float deltaTime)
{
  auto keyEvents = game->getInput()->getKeyEvents();
  for(auto event : keyEvents)
  {
    if(event->action != GLFW_PRESS || event->keyCode != GLFW_KEY_ENTER)
      continue;

    syukatsuGame->setScene(new PlayScene(syukatsuGame, select));    
  }

  if(game->getInput()->isKeyPressed(GLFW_KEY_LEFT))
  {
    select = (select - 1 + STAGE_NUM) % STAGE_NUM;
    field->setLane(select);
  }

  if(game->getInput()->isKeyPressed(GLFW_KEY_RIGHT))
  {
    select = (select + 1) % STAGE_NUM;
    field->setLane(select);
  }

  auto mouseEvent = syukatsuGame->getInput()->getMouseEvent();  

  if ( mouseEvent->action == GLFW_PRESS)
  {
    Vector2 touch(mouseEvent->x, mouseEvent->y);
    Vector2 menuTouch = menuCamera->screenToWorld(touch);  //メニュー画面のタッチ位置
    for(int i=0; i<STAGE_NUM; i++)
    {
      if( !icons[i]->inRegion(menuTouch) )
        continue;
    
      select = i;
      field->setLane(select);
    }   
  }
}

static void setting3D()
{
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

static void setting2D()
{
  glDisable(GL_DEPTH_TEST);  //これがあると2Dでは, 透過画像が使えないので消す
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
}

void StageSelectScene::render(float deltaTime)
{
  const int PUSH_ATTRIB_INFO_2D = GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT;
  
  glPushAttrib(PUSH_ATTRIB_INFO_2D);
  glPushMatrix();
  setting2D();
  menuCamera->setViewportAndMatrices();  
  batcher->beginBatch(Assets::selectAtlas);  
  for(int i=0; i<STAGE_NUM; i++)
  {
    float cx = icons[i]->lowerLeft.x+icons[i]->size.x/2;
    float cy = icons[i]->lowerLeft.y+icons[i]->size.y/2;
    batcher->drawSprite( cx, cy, icons[i]->size.x, icons[i]->size.y, icons[i]->image);    
    //選択中のハイライト表示
    if( i == select)
      batcher->drawSprite( cx, cy, icons[i]->size.x, icons[i]->size.y, Assets::highLight);
  }  
  batcher->endBatch();
  glPopMatrix();
  glPopAttrib();
  
  
  glPushAttrib(PUSH_ATTRIB_INFO_2D);
  glPushMatrix();
  setting2D();
  titleCamera->setViewportAndMatrices();
  std::string title = "Select Stage Number";
  float CharSize = TITLE_WINDOW_HEIGHT/3;
  MessageManager::drawBitmapString("Select Stage Number",
                                   Vector2(-TITLE_WINDOW_WIDTH/2, 0)
                                   , CharSize );
  glPopMatrix();
  glPopAttrib();

  
  //フィールドの描画
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_COLOR_MATERIAL);
  glPushMatrix();
  setting3D();
  camera->setViewportAndMatrices();   
  field->render(deltaTime);
  glPopMatrix();
  glPopAttrib();
  
}
