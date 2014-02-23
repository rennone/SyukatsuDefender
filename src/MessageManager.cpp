#include "MessageManager.h"
#include "Message.h"
#include <syukatsu/syukatsu.h>
#include <syukatsu/GLFW/glfw3.h>
#include "Assets.h"
#include "Character.h"
#include <iostream>
using namespace std;


//インスタントで使う,共通のバッチャ.
//複数のテクスチャとためとくようでは無く, 文字列, フレーム描画用
//cppファイルに直打ちしたくないから getterを用意
SpriteBatcher* MessageManager::getSpriteBatcher()
{
  static SpriteBatcher batcher(100);
  return &batcher;
}

SpriteBatcher3D* MessageManager::getSpriteBatcher3D()
{
  static SpriteBatcher3D batcher(100);
  return &batcher;
}

//-------------------グローバルなインスタンスの取得------------------------------//
//ぶっちゃけなくてもいい.
MessageManager* MessageManager::getInstance()
{
  //グローバルにアクセスできるメッセージを100個確保しておく
  static MessageManager instance("MessageManager", NULL, 100);
  
  return &instance;
}

void MessageManager::drawFrame
(const Information::FrameType &type, const Vector2 &upperLeft,const Vector2 &size, const TextColor &frameColor, const float &lineWidth)
{
  glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT | GL_ENABLE_BIT);
  glColor3d(frameColor.r, frameColor.g, frameColor.b);
  auto batcher = getSpriteBatcher();
  
  batcher->beginBatch(Assets::frameAtlas);
  batcher->drawSprite(upperLeft.x+size.x/2, upperLeft.y         , size.x+lineWidth, lineWidth, Assets::frameFillBackground);//上
  batcher->drawSprite(upperLeft.x+size.x/2, upperLeft.y-size.y  , size.x+lineWidth, lineWidth, Assets::frameFillBackground);//下
  batcher->drawSprite(upperLeft.x         , upperLeft.y-size.y/2, lineWidth, size.y+lineWidth/2, Assets::frameFillBackground);//左
  batcher->drawSprite(upperLeft.x+size.x  , upperLeft.y-size.y/2, lineWidth, size.y+lineWidth/2, Assets::frameFillBackground);//右  
  batcher->endBatch();  
  glPopAttrib();  
  /*
  //直線的なフレームしか描画できない上, glBeginを使っているので遅い
  glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT);
  glColor4d(frameColor.r, frameColor.g, frameColor.b, frameColor.a);
  glLineWidth(lineWidth);
  glBegin(GL_LINE_LOOP);
  glVertex2d(upperLeft.x, upperLeft.y);
  glVertex2d(upperLeft.x, upperLeft.y - size.y);
  glVertex2d(upperLeft.x+size.x, upperLeft.y - size.y);
  glVertex2d(upperLeft.x+size.x, upperLeft.y);
  glEnd();  
  glPopAttrib();
  */
}

void MessageManager::drawFillFrame
(const Information::FrameType &type,const Vector2 &upperLeft,const Vector2 &size, const TextColor &frameColor, const TextColor &fillColor,const float &lineWidth)
{
  auto batcher = getSpriteBatcher();
  glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT | GL_ENABLE_BIT);
  glColor4d(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
  batcher->beginBatch(Assets::frameAtlas);
  batcher->drawSprite(upperLeft.x+size.x/2, upperLeft.y-size.y/2, size.x-lineWidth, size.y-lineWidth, Assets::frameFillBackground);  //上
  batcher->endBatch();  
  
  glColor4d(frameColor.r, frameColor.g, frameColor.b, frameColor.a);  
  batcher->beginBatch(Assets::frameAtlas);
  batcher->drawSprite(upperLeft.x+size.x/2, upperLeft.y         , size.x+lineWidth, lineWidth, Assets::frameFillBackground);//上
  batcher->drawSprite(upperLeft.x+size.x/2, upperLeft.y-size.y  , size.x+lineWidth, lineWidth, Assets::frameFillBackground);//下  
  batcher->drawSprite(upperLeft.x         , upperLeft.y-size.y/2, lineWidth, size.y+lineWidth/2, Assets::frameFillBackground);//左
  batcher->drawSprite(upperLeft.x+size.x  , upperLeft.y-size.y/2, lineWidth, size.y+lineWidth/2, Assets::frameFillBackground);//右    
  batcher->endBatch();  
  glPopAttrib();  
  /*
  //直線的なフレームしか描画できない上, glBeginを使っているので遅い
  glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT);
  glColor4d(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
  glBegin(GL_POLYGON);
  glVertex2d(upperLeft.x, upperLeft.y);
  glVertex2d(upperLeft.x+size.x, upperLeft.y);
  glVertex2d(upperLeft.x+size.x, upperLeft.y-size.y);
  glVertex2d(upperLeft.x, upperLeft.y-size.y);
  glEnd();

  glColor4d(frameColor.r, frameColor.g, frameColor.b, frameColor.a);
  glLineWidth(lineWidth);
  glBegin(GL_LINE_LOOP);
  glVertex2d(upperLeft.x, upperLeft.y);
  glVertex2d(upperLeft.x, upperLeft.y - size.y);
  glVertex2d(upperLeft.x+size.x, upperLeft.y - size.y);
  glVertex2d(upperLeft.x+size.x, upperLeft.y);
  glEnd();
  glPopAttrib();
  */
}

//static SpriteBatcher batcher(100);
//bitmapFontによる, 文字列描画(英字のみ)
//自由な方向に描画させる為にttfファイルの代わりに使えるかも => すでに3Dは使ってるので, この関数はあまり意味ない
void MessageManager::drawBitmapString
(const string &str, const Vector2 &point, const float &size, const TextColor &color)
{
  //同時に描画できるのは100文字まで
  auto batcher = getSpriteBatcher();
//  auto batcher = getSpriteBatcher3D();
  
  glPushAttrib( GL_ENABLE_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);
  
  glColor4f(color.r, color.g, color.b, color.a);
  batcher->beginBatch(Assets::bitmapFont);
  float dx = 0.6*size; //幅を狭める為の重み(3Dではdepthテストのため1しか無理)
  float x = point.x + 0.5*size;
  float y = point.y + 0.5*size;
  
  for(int i=0; i<str.size(); i++)
  {
    if ( str[i] == '\n')
    {
      y -= size;
      x = point.x + 0.5*size;
      continue;
    }

    batcher->drawSprite(x, y, size, size, Assets::bitmapChar[(int)str[i]]);
    x += dx;
  }
  batcher->endBatch();
  
  glPopAttrib();
}

//未実装
void MessageManager::drawBitmapString
(const string &str, const Vector3 &position, const Vector3 &normal, const float &size, const TextColor &color, const int &rotateDegree)
{
  auto batcher = getSpriteBatcher();
  glPushAttrib(GL_CURRENT_BIT);
  glPopAttrib();
}

//------------------------------コンストラクタ------------------------------//
MessageManager::MessageManager(string name, SyukatsuGame *game, int _maxMessage)
  :
  Actor(name, game)
  ,msgIndex(0)
  ,effectMsgIndex(0)
  ,maxMessage(_maxMessage)
{
  instantMessages = new Message*[maxMessage];
  effectMessages = new EffectMessage*[maxMessage];
  for(int i=0; i<maxMessage; i++)
  {
    instantMessages[i] = new Message();
    instantMessages[i]->setStatus(Actor::NoUse);

    effectMessages[i] = new EffectMessage();
    effectMessages[i]->setStatus(Actor::NoUse);
  }  
}

//------------------------------デストラクタ------------------------------//
MessageManager::~MessageManager()
{
  for(int i=0; i<maxMessage; i++)
  {
    delete instantMessages[i];
    delete effectMessages[i];
  }
}

void MessageManager::update(float deltaTime)
{  
  for(int i=0; i<maxMessage; i++)
  {
    if(effectMessages[i]->getStatus() == Actor::NoUse)
      continue;
    effectMessages[i]->update(deltaTime);    
  }  
}

void MessageManager::reset()
{
  for(int i=0; i<maxMessage; i++)
  {
    instantMessages[i]->setStatus(Actor::NoUse);
    effectMessages[i]->target = NULL;
    effectMessages[i]->setStatus(Actor::NoUse);
  }
}

void MessageManager::render3DMessage(float deltaTime, Vector3 cameraPos, Vector3 cameraLook)
{  
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
  glDisable(GL_LIGHTING);
  
  //通常メッセージの描画
  for(int i=0; i<maxMessage; i++)
  {
    if(instantMessages[i]->getStatus() == Actor::NoUse || !instantMessages[i]->in3D)
      continue;

    //NoUseでない3Dメッセージの描画
    instantMessages[i]->render(deltaTime, cameraPos, cameraLook);
    //instantメッセージは毎フレームリセットする
    instantMessages[i]->setStatus(Actor::NoUse);
  }  
  msgIndex = 0; //0番から検索する為
  
  for(int i=0; i<maxMessage; i++)
  {
    if(effectMessages[i]->getStatus() == Actor::NoUse || !effectMessages[i]->in3D)
      continue;

    effectMessages[i]->render(deltaTime, cameraPos, cameraLook);
  }
  effectMsgIndex = 0; //0番から検索する為
  glPopAttrib();
}

void MessageManager::render2DMessage(float deltaTime)
{  
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);  
  glDisable(GL_LIGHTING);
  
  for(int i=0; i<maxMessage; i++)
  {
    if( instantMessages[i]->getStatus() == Actor::NoUse  || instantMessages[i]->in3D )
      continue;

    glPushMatrix();
    Vector3 pos = instantMessages[i]->position;
    auto color = instantMessages[i]->color;
    glColor4f(color.r, color.g, color.b, color.a);
    
    glTranslatef(pos.x, pos.y, 0);    
    Assets::messageFont->render(instantMessages[i]->text.c_str());
        
    instantMessages[i]->setStatus(Actor::NoUse);
    glPopMatrix();
  }
  msgIndex = 0;

  for(int i=0; i<maxMessage; i++)
  {
    if(effectMessages[i]->getStatus() == Actor::NoUse || effectMessages[i]->in3D)
      continue;

    glPushMatrix();
    
    Vector3 pos = effectMessages[i]->position;
    auto color = effectMessages[i]->color;
    glColor4f(color.r, color.g, color.b, color.a);
    
    glTranslatef(pos.x, pos.y, 0);    
    Assets::messageFont->render(effectMessages[i]->text.c_str());
    
    glPopMatrix();
  }
  effectMsgIndex = 0; //0番から検索する為
  glPopAttrib();
  glPopAttrib();
}

void MessageManager::render3DMessageIn2DScreen(float deltaTime, Camera3D *camera, Camera2D *camera2)
{
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);  
  glDisable(GL_LIGHTING);
  
  for(int i=0; i<maxMessage; i++)
  {
    if(instantMessages[i]->getStatus() == Actor::NoUse)
      continue;

    instantMessages[i]->renderWith2D(deltaTime, camera, camera2);
    instantMessages[i]->setStatus(Actor::NoUse);
  }
  msgIndex = 0;
  
  for(int i=0; i<maxMessage; i++)
  {
    if(effectMessages[i]->getStatus() == Actor::NoUse)
      continue;

    effectMessages[i]->renderWith2D(deltaTime, camera, camera2);
  }
  effectMsgIndex = 0;
  
  glPopAttrib();  
}

Message* MessageManager::getNewMessage()
{
  while(msgIndex<maxMessage && instantMessages[msgIndex]->getStatus() != Actor::NoUse)  
    msgIndex++;  

  if(msgIndex >= maxMessage)
  {
    cout << "no message is avalable" << endl;
    return NULL;
  }

  return instantMessages[msgIndex];
}

EffectMessage* MessageManager::getNewEffectMessage()
{
  while(effectMsgIndex < maxMessage && effectMessages[effectMsgIndex]->getStatus() != Actor::NoUse)  
    effectMsgIndex++;  

  if(effectMsgIndex >= maxMessage)
  {
    cout << "no message is avalable" << endl;
    return NULL;
  }

  return effectMessages[effectMsgIndex];
}

void MessageManager::drawMessage(string text, Vector3 position, TextColor color)
{  
  auto message = getNewMessage();
  if(message == NULL)
    return;

  message->setStatus(Actor::Action);
  message->in3D = true;  
  message->setMessage(text, position, color);  
}

void MessageManager::drawMessage(string text, Vector2 point, TextColor color)
{
  auto message = getNewMessage();
  if(message == NULL)
    return;

  message->setStatus(Actor::Action);
  message->in3D = false;
  message->setMessage(text, Vector3(point.x, point.y, 0), color);  
}

void MessageManager::effectMessage(string text, Vector3 position, float limit, TextColor color)
{
  auto message = getNewEffectMessage();
  if( message == NULL)
    return;

  color.a = 1.0;//effectMessageは最初は完全不透明
  message->setStatus(Actor::Action);
  message->setMessage(text, position, color);
  message->in3D = true;  
  message->setEffect(limit); 
}

void MessageManager::effectMessage(string text, Vector2 point, float limit, TextColor color)
{
  auto message = getNewEffectMessage();
  if( message == NULL)
    return;

  color.a = 1.0;//effectMessageは最初は完全不透明
  message->setStatus(Actor::Action);
  message->setMessage(text, Vector3(point.x, point.y, 0), color);
  message->in3D = false;  
  message->setEffect(limit);
}

void MessageManager::effectMessage(string text, Character *target, float limit, TextColor color, Vector3 offsetFromCharacter)
{
  auto message = getNewEffectMessage();
  if( message == NULL)
    return;

  color.a = 1.0; //effectMessageは最初は完全不透明
  message->setStatus(Actor::Action);
  message->setMessage(text, target->getPosition(), color);
  message->in3D = true;  
  message->setEffect(limit, target, offsetFromCharacter);
}


/*
  //テクスチャをつかったフレームの描画
//フレームの描画
void MessageManager::drawFrame
(const Information::FrameType &type, const Vector2 &upperLeft,const Vector2 &size, const TextColor &frameColor, const float &lineWidth)
{
  const float lineSize = min(lineWidth, min(size.x/2, size.y/2));
  
  const float sizeX[] = {lineSize, size.x-2*lineSize, lineSize};
  const float sizeY[] = {lineSize, size.y-2*lineSize, lineSize};
  float X[3], Y[3];
  float sumX=0, sumY=0;
  
  for ( int i=0; i<3; i++)
  {
    X[i] = upperLeft.x + sizeX[i]/2+sumX;
    Y[i] = upperLeft.y - sizeY[i]/2+sumY;
    sumX += sizeX[i];
    sumY -= sizeY[i];
  }

  glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT | GL_ENABLE_BIT);
  glColor3d(frameColor.r, frameColor.g, frameColor.b);
  auto batcher = getSpriteBatcher();

  auto edgeFrame       = Assets::frameEdge[type];
  auto horizontalFrame = Assets::frameHorizontal[type];
  auto verticalFrame   = Assets::frameVertical[type];

  batcher->beginBatch(Assets::frameAtlas);
  for(int i=0; i<2; i++)
  {
    batcher->drawSprite(X[1], Y[2*i], sizeX[1], (1-2*i)*sizeY[0], Assets::frameHorizontal[type]);
    batcher->drawSprite(X[2*i],Y[1], (1-2*i)*sizeX[0],  sizeY[1], Assets::frameVertical[type]);
  }

  int dx[] = {1, -1, -1,  1};
  int dy[] = {1,  1, -1, -1};
  for(int i=0; i<4; i++)
  {
    batcher->drawSprite(X[(i+1)&2], Y[i&2], dx[i]*sizeX[0], dy[i]*sizeY[0], Assets::frameEdge[type]);
  }
  
  batcher->endBatch();  
  glPopAttrib();  
}

//フレームの描画
void MessageManager::drawFillFrame
(const Information::FrameType &type,const Vector2 &upperLeft,const Vector2 &size, const TextColor &frameColor, const TextColor &fillColor,const float &lineWidth)
{
  const float lineSize = min(lineWidth, min(size.x/2, size.y/2));  
  const float sizeX[] = {lineSize, size.x-2*lineSize, lineSize};
  const float sizeY[] = {lineSize, size.y-2*lineSize, lineSize};
  
  float X[3], Y[3];
  float sumX=0, sumY=0;
  
  for ( int i=0; i<3; i++)
  {
    X[i] = upperLeft.x + sizeX[i]/2+sumX;
    Y[i] = upperLeft.y - sizeY[i]/2+sumY;
    sumX += sizeX[i];
    sumY -= sizeY[i];
  }

  glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT | GL_ENABLE_BIT);
  glColor3d(fillColor.r, fillColor.g, fillColor.b);
  auto batcher = getSpriteBatcher();

  auto edgeFillFrame       = Assets::frameFillEdge[type];
  auto horizontalFillFrame = Assets::frameFillHorizontal[type];
  auto verticalFillFrame   = Assets::frameFillVertical[type];
  batcher->beginBatch(Assets::frameAtlas);
  for(int i=0; i<2; i++)
  {
    batcher->drawSprite(X[1], Y[2*i], sizeX[1], (1-2*i)*sizeY[0], horizontalFillFrame);
    batcher->drawSprite(X[2*i],Y[1], (1-2*i)*sizeX[0],  sizeY[1], verticalFillFrame);   
  }

  int dx[] = {1, -1, -1,  1};
  int dy[] = {1,  1, -1, -1};
  for(int i=0; i<4; i++)
  {
    batcher->drawSprite(X[(i+1)&2], Y[i&2], dx[i]*sizeX[0], dy[i]*sizeY[0], edgeFillFrame);
  }

  batcher->drawSprite(upperLeft.x+size.x/2, upperLeft.y-size.y/2, size.x-2*lineSize, size.y-2*lineSize, Assets::frameFillBackground);
  batcher->endBatch();  
  glPopAttrib();

  drawFrame(type, upperLeft, size, frameColor);
}
*/
