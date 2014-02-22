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

//フレームの描画
void MessageManager::drawFrame
(const Vector2 &upperLeft,const Vector2 &size, const float &lineWidth)
{
  const float sizeX[] = {lineWidth, size.x-2*lineWidth, lineWidth};
  const float sizeY[] = {lineWidth, size.y-2*lineWidth, lineWidth};
  float X[3], Y[3];
  float sumX=0, sumY=0;
  
  for ( int i=0; i<3; i++)
  {
    X[i] = upperLeft.x + sizeX[i]/2+sumX;
    Y[i] = upperLeft.y - sizeY[i]/2+sumY;
    sumX += sizeX[i];
    sumY -= sizeY[i];
  }

  glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);

  auto batcher = getSpriteBatcher();
  
  batcher->beginBatch(Assets::playAtlas);
  for(int i=0; i<2; i++)
  {
    batcher->drawSprite(X[1], Y[2*i], sizeX[1], (1-2*i)*sizeY[0], Assets::frameHorizontal);
    batcher->drawSprite(X[2*i],Y[1], (1-2*i)*sizeX[0],  sizeY[1], Assets::frameVertical);
  }

  int dx[] = {1, -1, -1,  1};
  int dy[] = {1,  1, -1, -1};
  for(int i=0; i<4; i++)
    batcher->drawSprite(X[(i+1)&2], Y[i&2], dx[i]*sizeX[0], dy[i]*sizeY[0], Assets::frameEdge);

  batcher->endBatch();
  
  glPopAttrib();
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
    /*
    batcher->drawSprite(Vector3(x, y, 1),
                        Vector3(1,0,0),
                        Vector3(0,1,0),
                        Vector2(size, size),
                        Assets::bitmapChar[(int)str[i]]);
    */
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
