#include "Message.h"
#include "Assets.h"
#include <syukatsu/syukatsu.h>
Message::Message(string _text, Vector3 _position, float _limit, bool _isFixed)
  :Actor("message", NULL)
  ,text(_text)
  ,position(_position)
  ,limitTime(_limit)
  ,isFixed(_isFixed)
  ,elapsedTime(0)
{
  auto b = Assets::messageFont->font->BBox(text.c_str());
  Vector3 lower(b.Lower().X(), b.Lower().Y(), b.Lower().Z());
  Vector3 upper(b.Upper().X(), b.Upper().Y(), b.Upper().Z());

  offset = 0.5*(lower + upper);

  
}

Message::~Message()
{
}

void Message::update(float deltaTime)
{
  elapsedTime += deltaTime;
  
  Actor::update(deltaTime);  
}

void Message::render(float deltaTime)
{
  glPushMatrix();  
  glTranslatef(position.x - offset.x, position.y-offset.y, position.z-offset.y);
  Assets::messageFont->render(text.c_str());  
  Actor::render(deltaTime);
  glPopMatrix();  
}

void Message::render(const Vector3 &cameraPos, float deltaTime)
{
  glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
  glDisable(GL_LIGHTING);  
  Vector2 initNormal(0,1); //元はz方向を向いている
  Vector3 dir = cameraPos - (position-offset); //回転後の方向ベクトル
  dir.normalize();
  float angle = -initNormal.angleTo(Vector2(dir.x, dir.z))*Vector2::TO_DEGREE; //角度を求める
  glPushMatrix();
  glTranslatef(position.x, position.y, position.z);
  glRotatef(angle, 0, 1, 0);
  glTranslatef(-offset.x, -offset.y, -offset.y);
  Assets::messageFont->render(text.c_str());  
  Actor::render(deltaTime);
  glPopMatrix();
  glPopAttrib();  
}

