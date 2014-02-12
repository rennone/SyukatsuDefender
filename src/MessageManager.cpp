#include "MessageManager.h"
#include "Message.h"
#include <syukatsu/syukatsu.h>
#include <syukatsu/GLFW/glfw3.h>
#include "Assets.h"
#include "Character.h"
#include <iostream>
using namespace std;

//------------------------------インスタンスの取得------------------------------//
MessageManager* MessageManager::getInstance()
{
  static MessageManager instance;
  
  return &instance;
}

//------------------------------コンストラクタ------------------------------//
MessageManager::MessageManager()
  :msgIndex(0)
  ,effectMsgIndex(0)
{
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

void MessageManager::_update(float deltaTime)
{  
  for(int i=0; i<maxMessage; i++)
  {
    if(effectMessages[i]->getStatus() == Actor::NoUse)
      continue;
    effectMessages[i]->update(deltaTime);    
  }  
}

void MessageManager::_reset()
{
  for(int i=0; i<maxMessage; i++)
  {
    instantMessages[i]->setStatus(Actor::NoUse);
    effectMessages[i]->target = NULL;
    effectMessages[i]->setStatus(Actor::NoUse);
  }
}

void MessageManager::_render3DMessage(float deltaTime, Vector3 cameraPos)
{  
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
  glDisable(GL_LIGHTING);
  
  //通常メッセージの描画
  for(int i=0; i<maxMessage; i++)
  {
    if(instantMessages[i]->getStatus() == Actor::NoUse || !instantMessages[i]->in3D)
      continue;

    //NoUseでない3Dメッセージの描画
    instantMessages[i]->render(deltaTime, cameraPos);
    //instantメッセージは毎フレームリセットする
    instantMessages[i]->setStatus(Actor::NoUse);
  }  
  msgIndex = 0; //0番から検索する為
  
  for(int i=0; i<maxMessage; i++)
  {
    if(effectMessages[i]->getStatus() == Actor::NoUse)
      continue;

    effectMessages[i]->render(deltaTime, cameraPos);
  }
  effectMsgIndex = 0; //0番から検索する為
  glPopAttrib();
}

void MessageManager::_render2DMessage(float deltaTime)
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
  
  glPopAttrib();
}

void MessageManager::_render3DMessageIn2DScreen(float deltaTime, Camera3D *camera, Camera2D *camera2)
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

void MessageManager::_drawMessage(string text, Vector3 position, TextColor color)
{  
  auto message = getNewMessage();
  if(message == NULL)
    return;

  message->setStatus(Actor::Action);
  message->in3D = true;  
  message->setMessage(text, position, color);  
}

void MessageManager::_drawMessage(string text, Vector2 point, TextColor color)
{
  auto message = getNewMessage();
  if(message == NULL)
    return;

  message->setStatus(Actor::Action);
  message->in3D = false;
  message->setMessage(text, Vector3(point.x, point.y, 0), color);  
}

void MessageManager::_effectMessage(string text, Vector3 position, float limit, TextColor color)
{
  auto message = getNewEffectMessage();
  if( message == NULL)
    return;

  color.a = 1.0;//effectMessageは最初は完全不透明
  message->setStatus(Actor::Action);
  message->setMessage(text, position, color);
  message->setEffect(limit); 
}

void MessageManager::_effectMessage(string text, Character *target, float limit, TextColor color, Vector3 offsetFromCharacter)
{
  auto message = getNewEffectMessage();
  if( message == NULL)
    return;

  color.a = 1.0; //effectMessageは最初は完全不透明
  message->setStatus(Actor::Action);
  message->setMessage(text, target->getPosition(), color);
  message->setEffect(limit, target, offsetFromCharacter);
}

//static SpriteBatcher batcher(100);
//bitmapFontによる, 文字列描画(英字のみ)
//自由な方向に描画させる為にttfファイルの代わりに使えるかも
void MessageManager::drawBitmapString(string str, Vector2 point, float size, TextColor color)
{
  //同時に描画できるのは100文字まで
  static SpriteBatcher batcher(100);

  glPushAttrib(GL_CURRENT_BIT);
  glColor4f(color.r, color.g, color.b, color.a);
  batcher.beginBatch(Assets::bitmapFont);

  for(int i=0; i<str.size(); i++)  
    batcher.drawSprite(point.x+(i*0.8+0.5)*size, point.y+0.5*size,
                        size, size, Assets::bitmapChar[(int)str[i]]);  
  batcher.endBatch();
  
  glPopAttrib();
}

void MessageManager::drawBitmapString(string str, Vector3 position, Vector3 normal, float size, TextColor color, int rotateDegree)
{
  static SpriteBatcher batcher(100);
  glPushAttrib(GL_CURRENT_BIT);
  glPopAttrib();
}
