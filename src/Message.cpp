#include "Message.h"
#include "Assets.h"
#include "Character.h"
#include <syukatsu/syukatsu.h>

Message::Message()
  :Actor("message", NULL)
  ,text("no text")
{  
}

Message::~Message()
{
}

void Message::render(float deltaTime, Vector3 cameraPos)
{
  /*
    呼び出す前に, GL_LIGHTGを消さないと, 色がつかない
   */
  
  Vector2 initNormal(0,1); //元はz方向を向いている
  Vector3 dir = cameraPos - (position-offset); //回転後の方向ベクトル    
  dir.normalize();
  Vector3 initNormal3(0,0,1);
  Vector3 axis = (-dir).cross(initNormal3);
    
  float angle  = -initNormal.angleTo(Vector2(dir.x, dir.z))*Vector2::TO_DEGREE; //角度を求める    
  float angle2 = initNormal.angleTo(Vector2(dir.y, dir.z))*Vector2::TO_DEGREE;  //角度を求める
  float angle3 = dir.angleTo(initNormal3)*Vector3::TO_DEGREE;
  glColor4f(color.r, color.g, color.b, alpha);
  
  glPushMatrix();
  glTranslatef(position.x, position.y, position.z);
  glRotatef(angle , 0, 1, 0);
//    glRotatef(angle2, 1, 0, 0);
//    glRotatef(-angle3, axis.x, axis.y, axis.z);
  glTranslatef(-offset.x, -offset.y, -offset.z);
    
  Assets::messageFont->render(text.c_str());
  glPopMatrix();
}

void Message::renderWith2D(float deltaTime, Camera3D *camera, Camera2D *camera2)
{  
  Vector2 point = camera->worldToScreen(position);
  glColor4f(color.r, color.g, color.b, alpha);
  
  glPushMatrix();
  point = camera2->screenToWorld(point);
  glTranslatef(point.x-offset.x, point.y-offset.y, 0);
  Assets::messageFont->render(text.c_str());
  glPopMatrix();
}

void Message::setMessage(string text, Vector3 position, TextColor color, float alpha)
{
  this->text = text;
  this->position = position;
  this->color = color;
  this->alpha = alpha;
  
  auto b = Assets::messageFont->font->BBox(text.c_str());
  Vector3 lower(b.Lower().X(), b.Lower().Y(), b.Lower().Z());
  Vector3 upper(b.Upper().X(), b.Upper().Y(), b.Upper().Z());
  offset = 0.5*(lower + upper); 
}


EffectMessage::EffectMessage()
  :Message()
  ,target(NULL)
  ,offsetFromCharacter(Vector3(0,0,0))
  ,limitTime(-1)
  ,elapsedTime(0)
{
}

EffectMessage::~EffectMessage()
{
}

void EffectMessage::update(float deltaTime)
{
  elapsedTime += deltaTime;
  alpha = (limitTime - elapsedTime)/limitTime;

  if(target != NULL)
  {
    position = target->getPosition() + offsetFromCharacter;
    position.y += target->getRadius()*2;
    offsetFromCharacter.y += 10*deltaTime;
  }
  else
  {
    position.y += deltaTime*10;
  }
  
  if(elapsedTime >= limitTime)
  {
    setStatus(Actor::NoUse);
    target = NULL;
  }
  
  Actor::update(deltaTime);  
}

void EffectMessage::setEffect(float limit, Character *_target, Vector3 _offsetFromCharacter)
{
  limitTime = limit;
  target = _target;
  offsetFromCharacter = _offsetFromCharacter;
  elapsedTime = 0;
}
