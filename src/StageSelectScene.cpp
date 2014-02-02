#include "StageSelectScene.h"
#include "Field.h"
#include "Information.h"
#include "Assets.h"
#include "PlayScene.h"
#include "Debugger.h"


static void LightSetting()
{
  glEnable(GL_LIGHTING);    
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHT2);
  glEnable(GL_LIGHT3);

  float edge = Field::cellNum*Field::cellSize*2;
//  GLfloat lightcol1[] = { 1.0, 0.7, 0.7, 1.0 };
  GLfloat lightpos1[] = { 0.0, edge/2, 0.0, 1.0 };
  GLfloat lightdir1[] = { 1.0, -1.0, 1.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos1);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightdir1);
//  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, lightcol1);
  
  GLfloat lightpos2[] = { edge, edge/2, edge, 1.0 };
  GLfloat lightdir2[] = { -1.0, -1.0, -1.0, 1.0 };
  glLightfv(GL_LIGHT1, GL_POSITION, lightpos2);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightdir2);
 
  GLfloat lightpos3[] = { 0.0, edge/2, edge, 1.0 };
  GLfloat lightdir3[] = { 1.0, -1.0, -1.0, 1.0 };
  glLightfv(GL_LIGHT2, GL_POSITION, lightpos3);
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightdir3);
 
  GLfloat lightpos4[] = { edge, edge/2, 0.0, 1.0 };
  GLfloat lightdir4[] = { -1.0, -1.0, 1.0, 1.0 };
  glLightfv(GL_LIGHT4, GL_POSITION, lightpos4);
  glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, lightdir4); 
}

StageSelectScene::StageSelectScene(SyukatsuGame *game)
  :SyukatsuScene(game)
  ,select(0)
{
  camera = new Camera3D(syukatsuGame->getWindow(), 1, 3000, 80);
  menuCamera = new Camera2D(syukatsuGame->getWindow(), MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
  
  int width, height;
  glfwGetFramebufferSize(syukatsuGame->getWindow(), &width, &height);
  cameraViewportSetting(width, height);
  
  float playWindowWidth  = width/2;
  float playWindowHeight = height*3.0/4.0;

  float menuWindowWidth  = width;
  float menuWindowHeight = height*1.0/4.0;
  float menuRatio = menuWindowWidth/(float)menuWindowHeight;
  
  MENU_WINDOW_HEIGHT = 100.0f;
  MENU_WINDOW_WIDTH  = MENU_WINDOW_HEIGHT*menuRatio;
  
  camera = new Camera3D(syukatsuGame->getWindow(), 1, 3000, 80);
  camera->setViewportWidth(playWindowWidth);
  camera->setViewportHeight(playWindowHeight);
  camera->setViewportPosition(width/2, height/3*2);

  camera->setLook(Vector3(Field::cellNum*Field::cellSize/2, 0, Field::cellNum*Field::cellSize/2));
  camera->setPosition(Vector3(0, Field::cellNum*Field::cellSize, 0));
  menuCamera = new Camera2D(syukatsuGame->getWindow(), MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT);
  menuCamera->setViewportWidth(menuWindowWidth);
  menuCamera->setViewportHeight(menuWindowHeight);
  menuCamera->setViewportPosition(menuWindowWidth/2, menuWindowHeight/2);
  
  field = new Field("sampleField", syukatsuGame);
  field->setLane(select);
  
  batcher = new SpriteBatcher(STAGE_NUM*2);
  
  float iconSize = MENU_WINDOW_WIDTH/(float)(STAGE_NUM*2+1);
  float y = -iconSize/2;
  for(int i=0; i<Information::BUILDING_NUM; i++)
  {
    float x = -MENU_WINDOW_WIDTH/2 + (2*i+1)*iconSize;
    icons[i] = new Icon(Vector2( x, y), Vector2( iconSize, iconSize), Assets::stageIcons[i]);
  }

  LightSetting();
}

StageSelectScene::~StageSelectScene()
{
  
}

void StageSelectScene::reshape(int width, int height)
{
  cameraViewportSetting(width, height);
}

void StageSelectScene::cameraViewportSetting(int width, int height)
{
  float playWindowWidth  = width/2;
  float playWindowHeight = height*3.0/4.0;

  float menuWindowWidth  = width;
  float menuWindowHeight = height*1.0/4.0;
  float menuRatio = menuWindowWidth/(float)menuWindowHeight;
  
  MENU_WINDOW_HEIGHT = 100.0f;
  MENU_WINDOW_WIDTH  = MENU_WINDOW_HEIGHT*menuRatio;

  camera->setViewportWidth(playWindowWidth);
  camera->setViewportHeight(playWindowHeight);
  camera->setViewportPosition(width/2, height/3*2);

  camera->setLook(Vector3(Field::cellNum*Field::cellSize/2, 0, Field::cellNum*Field::cellSize/2));
  camera->setPosition(Vector3(0, Field::cellNum*Field::cellSize, 0));
  
  menuCamera->setFrustumSize(Vector2(MENU_WINDOW_WIDTH, MENU_WINDOW_HEIGHT));
  menuCamera->setViewportWidth(menuWindowWidth);
  menuCamera->setViewportHeight(menuWindowHeight);
  menuCamera->setViewportPosition(menuWindowWidth/2, menuWindowHeight/2);
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
  Debugger::drawDebugInfo("StageSelectScene.cpp", "select", select);
}

void StageSelectScene::render(float deltaTime)
{
  //フィールドの描画
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_COLOR_MATERIAL);
  camera->setViewportAndMatrices();
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA_TEST);
  LightSetting();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  field->render(deltaTime);
  glPopAttrib();

  
  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT);
  glDisable(GL_DEPTH_TEST);  //これがあると2Dでは, 透過画像が使えないので消す
  glDisable(GL_LIGHTING);
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
  glPopAttrib();
}
