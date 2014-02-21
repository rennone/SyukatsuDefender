#include "Message.h"
#include "Assets.h"
#include "Character.h"
#include <syukatsu/syukatsu.h>

//Messageクラスで共通して使う為のバッチャ, 最大で一度に100文字病が可能
SpriteBatcher3D Message::batcher(100);

Message::Message()
  :Actor("message", NULL)
  ,text("no text")
  ,in3D(false)
{
}

Message::~Message()
{
}

void Message::render(float deltaTime, Vector3 cameraPos, Vector3 cameraLook)
{
  /*
    呼び出す前に, GL_LIGHTGを消さないと, 色がつかない
   */
  glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);
  glColor4f(color.r, color.g, color.b, color.a);  
  
  const Vector3 up(0,1,0);  
  Vector3 normal = cameraPos - cameraLook;  
  auto axis1 = up.cross(normal);
  axis1.normalize();
  auto axis2 = normal.cross(axis1);
  axis2.normalize();

  const float size = pow(normal.length(),0.5);  //カメラとの距離により,文字の大きさを変える
  batcher.beginBatch(Assets::bitmapFont);
  for(int i=0; i<text.size(); i++)
  {    
    batcher.drawSprite(position+( i*0.7+0.5-text.size()/2)*size*axis1, axis1, axis2, Vector2(size, size), Assets::bitmapChar[(int)text[i]]);
  }  
  batcher.endBatch();
  
  glPopAttrib();
}

//3Dメッセージを2Dに射影して表示 => 使っていない
void Message::renderWith2D(float deltaTime, Camera3D *camera, Camera2D *camera2)
{
  Vector2 point = camera->worldToScreen(position);
  glColor4f(color.r, color.g, color.b, color.a);
  
  glPushMatrix();
  point = camera2->screenToWorld(point);
  glTranslatef(point.x-offset.x, point.y-offset.y, 0);
  Assets::messageFont->render(text.c_str());
  glPopMatrix();
}

void Message::setMessage(string text, Vector3 position, TextColor color)
{
  this->text = text;
  this->position = position;
  this->color = color;
  
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
  color.a = (limitTime - elapsedTime)/limitTime;

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
