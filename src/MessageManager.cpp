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
    Vector3 initNormal3(0,0,1);
    Vector3 axis = (-dir).cross(initNormal3);
    
    float angle  = -initNormal.angleTo(Vector2(dir.x, dir.z))*Vector2::TO_DEGREE; //角度を求める    
    float angle2 = initNormal.angleTo(Vector2(dir.y, dir.z))*Vector2::TO_DEGREE;  //角度を求める
    float angle3 = dir.angleTo(initNormal3)*Vector3::TO_DEGREE;
    glColor4f(textColors[messages[i]->color].r,
              textColors[messages[i]->color].g,
              textColors[messages[i]->color].b,
              messages[i]->alpha);
    glPushMatrix();
    glTranslatef(messages[i]->position.x, messages[i]->position.y, messages[i]->position.z);
//    glRotatef(angle , 0, 1, 0);
//    glRotatef(angle2, 1, 0, 0);
    glRotatef(-angle3, axis.x, axis.y, axis.z);
    glTranslatef(-messages[i]->offset.x, -messages[i]->offset.y, -messages[i]->offset.z);
    
    Assets::messageFont->render(messages[i]->text.c_str());
    glPopMatrix();

    //エフェクト用のメッセージ以外は消す
    if(messages[i]->limitTime <= 0)
      messages[i]->setStatus(Actor::NoUse);
  }
  
  glPopAttrib();
  msgIndex = 0;  
}

#include "Debugger.h"

static Vector2 worldToScreen(Camera3D *camera, const Vector3 &worldPoint)
{
  const Vector3 look = camera->getLook();
  const Vector3 up   = camera->getUp();
  const Vector3 position = camera->getPosition();

  //カメラ座標に置ける各軸ベクトル
  Vector3 camZ = look - position;
  camZ.normalize();
  Vector3 camY = up - camZ.dot(up)*camZ;
  camY.normalize();
  Vector3 camX = camZ.cross(camY);
  camX.normalize();

  const Vector3 posDir = worldPoint - position;
  float elemZ = posDir.dot(camZ); //z成分
  float elemY = posDir.dot(camY);
  float elemX = posDir.dot(camX);

  float fov = 60; //60°で固定

  float ratio = 640.0/480.0;
  //今の位置の視錐台の幅
  float Wheight = 2*elemZ*tan(0.5*fov*Vector3::TO_RADIANS);
  float Wwidth = ratio*Wheight;
Debugger::drawDebugInfo("MessageManger.cpp", "Wsize", Vector3(Wwidth, Wheight, elemZ));
Debugger::drawDebugInfo("MessageManger.cpp", "WPos", Vector3(elemX, elemY, elemZ));

  //正規化する
  float Nheight = elemY/Wheight;
  
  float Nwidth  = elemX/Wwidth;
  return Vector2(Nwidth, Nheight);
}

void MessageManager::render2(float deltaTime, Camera3D *camera, Camera2D *camera2)
{
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);  
  glDisable(GL_LIGHTING);  
  for(int i=0; i<maxMessage; i++)
  {
    if(messages[i]->getStatus() == Actor::NoUse)
      continue;

Vector2 point = camera->worldToScreen(messages[i]->position);
//worldToScreen(camera, messages[i]->position);
    glColor4f(textColors[messages[i]->color].r,
              textColors[messages[i]->color].g,
              textColors[messages[i]->color].b,
              messages[i]->alpha);
    glPushMatrix();
point = camera2->screenToWorld(point);


    glTranslatef(point.x-messages[i]->offset.x, point.y-messages[i]->offset.y, 0);
    Assets::messageFont->render(messages[i]->text.c_str());
    glPopMatrix();

    //エフェクト用のメッセージ以外は消す
    if(messages[i]->limitTime <= 0)
      messages[i]->setStatus(Actor::NoUse);
  }
  glPopAttrib();
  msgIndex = 0;  
}

void MessageManager::drawMessage(string text, Vector3 position, float alpha, TextColors::TextColors color)
{  
  while(msgIndex<maxMessage && messages[msgIndex]->getStatus() != Actor::NoUse)  
    msgIndex++;  

  if(msgIndex >= maxMessage)
  {    
    cout << "no message is avalable" << endl;
    return;    
  }

  messages[msgIndex]->setStatus(Actor::Action);
  messages[msgIndex]->setMessage(text, position, -1, color, alpha);  
}

void MessageManager::effectMessage(string text, Vector3 position, float limit, TextColors::TextColors color)
{
}



