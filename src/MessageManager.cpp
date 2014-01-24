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

void MessageManager::_render3DMessage(float deltaTime, Vector3 cameraPos)
{  
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);  
  glDisable(GL_LIGHTING);  
  for(int i=0; i<maxMessage; i++)
  {
    if(instantMessages[i]->getStatus() == Actor::NoUse || !instantMessages[i]->in3D)
      continue;

    instantMessages[i]->render(deltaTime, cameraPos);
    instantMessages[i]->setStatus(Actor::NoUse);
  }
  msgIndex = 0;
  
  for(int i=0; i<maxMessage; i++)
  {
    if(effectMessages[i]->getStatus() == Actor::NoUse)
      continue;

    effectMessages[i]->render(deltaTime, cameraPos);
  }
  effectMsgIndex = 0;
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

void MessageManager::_drawMessage(string text, Vector3 position, float alpha, TextColor color)
{  
  auto message = getNewMessage();
  if(message == NULL)
    return;

  message->setStatus(Actor::Action);
  message->in3D = true;  
  message->setMessage(text, position, color, alpha);  
}

void MessageManager::_drawMessage(string text, Vector2 point, float alpha, TextColor color)
{
  auto message = getNewMessage();
  if(message == NULL)
    return;

  message->setStatus(Actor::Action);
  message->in3D = false;
  message->setMessage(text, Vector3(point.x, point.y, 0), color, alpha);  
}

void MessageManager::_effectMessage(string text, Vector3 position, float limit, TextColor color)
{
  /*
  while(effectMsgIndex<maxMessage && effectMessages[effectMsgIndex]->getStatus() != Actor::NoUse)  
    effectMsgIndex++;  

  if(effectMsgIndex >= maxMessage)
  {    
    cout << "no message is avalable" << endl;
    return;
  }
  */
  auto message = getNewEffectMessage();
  if( message == NULL)
    return;

  const float alpha = 1.0;
  message->setStatus(Actor::Action);
  message->setMessage(text, position, color, alpha);
  message->setEffect(limit);
  /*
  effectMessages[effectMsgIndex]->setStatus(Actor::Action);
  effectMessages[effectMsgIndex]->setMessage(text, position, color, 1);
  effectMessages[effectMsgIndex]->setEffect(limit);
  */
}

void MessageManager::_effectMessage(string text, Character *target, float limit, TextColor color, Vector3 offsetFromCharacter)
{
  auto message = getNewEffectMessage();
  if( message == NULL)
    return;

  const float alpha = 1.0;
  message->setStatus(Actor::Action);
  message->setMessage(text, target->getPosition(), color, alpha);
  message->setEffect(limit, target, offsetFromCharacter);
/*
  effectMessages[effectMsgIndex]->setStatus(Actor::Action);
  effectMessages[effectMsgIndex]->setMessage(text, target->getPosition(), color, 1);
  effectMessages[effectMsgIndex]->setEffect(limit, target, offsetFromCharacter);
*/
}
