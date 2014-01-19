#include "MessageManager.h"
#include "Message.h"
#include <syukatsu/syukatsu.h>
#include <syukatsu/GLFW/glfw3.h>
#include "Assets.h"
#include "Character.h"
#include <iostream>
using namespace std;

TextColor MessageManager::textColors[TextColors::COLORNUM] = 
{
  TextColor(1,0,0,1),
  TextColor(0,0,1,1),
  TextColor(0,1,0,1),
  TextColor(1,1,0,1),
  TextColor(0,0,0,1),
  TextColor(1,1,1,1)  
};

int MessageManager::msgIndex = 0;
int MessageManager::effectMsgIndex = 0;
Message *MessageManager::instantMessages[maxMessage];
EffectMessage *MessageManager::effectMessages[maxMessage];

void MessageManager::initialize()
{
  for(int i=0; i<maxMessage; i++)
  {
    instantMessages[i] = new Message();
    instantMessages[i]->setStatus(Actor::NoUse);

    effectMessages[i] = new EffectMessage();
    effectMessages[i]->setStatus(Actor::NoUse);
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

void MessageManager::render(float deltaTime, Vector3 cameraPos)
{  
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);  
  glDisable(GL_LIGHTING);  
  for(int i=0; i<maxMessage; i++)
  {
    if(instantMessages[i]->getStatus() == Actor::NoUse)
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

void MessageManager::render2(float deltaTime, Camera3D *camera, Camera2D *camera2)
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

void MessageManager::drawMessage(string text, Vector3 position, float alpha, TextColors::TextColors color)
{  
  while(msgIndex<maxMessage && instantMessages[msgIndex]->getStatus() != Actor::NoUse)  
    msgIndex++;  

  if(msgIndex >= maxMessage)
  {    
    cout << "no message is avalable" << endl;
    return;    
  }

  instantMessages[msgIndex]->setStatus(Actor::Action);
  instantMessages[msgIndex]->setMessage(text, position, textColors[color], alpha);  
}

void MessageManager::effectMessage(string text, Vector3 position, float limit, TextColors::TextColors color)
{
  
  while(effectMsgIndex<maxMessage && effectMessages[effectMsgIndex]->getStatus() != Actor::NoUse)  
    effectMsgIndex++;  

  if(effectMsgIndex >= maxMessage)
  {    
    cout << "no message is avalable" << endl;
    return;
  }

  effectMessages[effectMsgIndex]->setStatus(Actor::Action);
  effectMessages[effectMsgIndex]->setMessage(text, position, textColors[color], 1);
  effectMessages[effectMsgIndex]->setEffect(limit);
}

void MessageManager::effectMessage(string text, Character *target, float limit, TextColors::TextColors color, Vector3 offsetFromCharacter)
{
  while(effectMsgIndex<maxMessage && effectMessages[effectMsgIndex]->getStatus() != Actor::NoUse)  
    effectMsgIndex++;  

  if(effectMsgIndex >= maxMessage)
  {    
    cout << "no message is avalable" << endl;
    return;
  }

  effectMessages[effectMsgIndex]->setStatus(Actor::Action);
  effectMessages[effectMsgIndex]->setMessage(text, target->getPosition(), textColors[color], 1);
  effectMessages[effectMsgIndex]->setEffect(limit, target, offsetFromCharacter);
}
