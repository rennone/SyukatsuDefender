#include "Message.h"
#include "Assets.h"
#include <syukatsu/syukatsu.h>

Message::Message()
  :Actor("message", NULL)
  ,text("no text")
  ,limitTime(-1)
  ,elapsedTime(0)
{  
}

Message::~Message()
{
}

void Message::update(float deltaTime)
{
  if( limitTime > 0 &&  (elapsedTime+=deltaTime) >= limitTime)
    setStatus(Actor::NoUse);
  
  Actor::update(deltaTime);  
}

void Message::setMessage(string text, Vector3 position, float limit, TextColors::TextColors color, float alpha)
{
  this->text = text;
  this->position = position;
  this->limitTime = limit;
  this->color = color;
  this->alpha = alpha;  
  elapsedTime = 0;
  
  auto b = Assets::messageFont->font->BBox(text.c_str());
  Vector3 lower(b.Lower().X(), b.Lower().Y(), b.Lower().Z());
  Vector3 upper(b.Upper().X(), b.Upper().Y(), b.Upper().Z());
  offset = 0.5*(lower + upper); 
}
