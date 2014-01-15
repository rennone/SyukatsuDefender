#include "MessageManager.h"
#include "Message.h"
#include <syukatsu/syukatsu.h>
#include <syukatsu/GLFW/glfw3.h>
#include "Assets.h"
#include <iostream>
using namespace std;

Color4 MessageManager::textColors[TextColors::COLORNUM] = 
{
  Color4(1,0,0,1),
  Color4(0,0,1,1),
  Color4(0,1,0,1),
  Color4(1,1,0,1),
  Color4(0,0,0,1),
  Color4(1,1,1,1)  
};

int MessageManager::msgIndex = 0;
Message *MessageManager::messages[maxMessage];

void MessageManager::initialize()
{
  for(int i=0; i<maxMessage; i++)
  {
    messages[i] = new Message();
    messages[i]->setStatus(Actor::NoUse);
  }  
}

void MessageManager::update(float deltaTime)
{
  for(int i=0; i<maxMessage; i++)
  {
    if(messages[i]->getStatus() == Actor::NoUse)
      continue;
    messages[i]->update(deltaTime);    
  }  
}

void MessageManager::render(float deltaTime, Vector3 cameraPos)
{
  
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);  
  glDisable(GL_LIGHTING);  
  for(int i=0; i<maxMessage; i++)
  {
    if(messages[i]->getStatus() == Actor::NoUse)
      continue;
   
    Vector2 initNormal(0,1); //元はz方向を向いている
    Vector3 dir = cameraPos - (messages[i]->position-messages[i]->offset); //回転後の方向ベクトル
    dir.normalize();
    float angle = -initNormal.angleTo(Vector2(dir.x, dir.z))*Vector2::TO_DEGREE; //角度を求める    
    float ang = floor(angle/90.0+0.5)*90;  

    glColor4f(textColors[messages[i]->color].r,
              textColors[messages[i]->color].g,
              textColors[messages[i]->color].b,
              messages[i]->alpha);    
    glPushMatrix();    
    glTranslatef(messages[i]->position.x, messages[i]->position.y, messages[i]->position.z);
    glRotatef(ang, 0, 1, 0);
    glTranslatef(-messages[i]->offset.x, -messages[i]->offset.y, -messages[i]->offset.y);
    
    Assets::messageFont->render(messages[i]->text.c_str());
    glPopMatrix();

    //エフェクト用のメッセージ以外は消す
    if(messages[i]->limitTime <= 0)
      messages[i]->setStatus(Actor::NoUse);
  }
  glPopAttrib();

  msgIndex = 0;  
}

  void MessageManager::drawMessage(string text, Vector3 position, float limit, TextColors::TextColors color, float alpha)
{  
  while(msgIndex<maxMessage && messages[msgIndex]->getStatus() != Actor::NoUse)  
    msgIndex++;  

  if(msgIndex >= maxMessage)
  {    
    cout << "no message is avalable" << endl;
    return;    
  }

  messages[msgIndex]->setStatus(Actor::Action);
  messages[msgIndex]->setMessage(text, position, limit, color, alpha);  
}




