#ifndef MESSAGE_H
#define MESSAGE_H

#include <math/Vector3.h>
#include <math/Vector2.h>
#include <string>
#include "Actor.h"
#include "Information.h"

class Character;
class Camera3D;
class Camera2D;
using namespace TextColors;
class Message : public Actor
{
  friend class MessageManager;
protected:
  Message();
  virtual ~Message();
  Message& operator=(const Message&) const;
  Vector3 position;
  string text;
  Vector3 offset;               //positionを文字の中心にする為のオフセット  
  float alpha;
  TextColor color;
  bool in3D;
public:
  void setMessage(string text, Vector3 position, TextColor color, float alpha);
  virtual void render(float deltaTime, Vector3 cameraPos);
  void renderWith2D(float deltaTime, Camera3D *camera, Camera2D *camera2);
};

class EffectMessage : public Message
{
  friend class MessageManager;
  bool trace;  
  
  Character *target;
  Vector3 offsetFromCharacter;  // あるキャラクターに追尾する時のキャラクターとのオフセット量
  
  float limitTime;  //消えるまでの時間 負だと消えない
  float elapsedTime;

  EffectMessage();  
  ~EffectMessage();
  EffectMessage& operator=(const EffectMessage&) const;  
public:
  void setEffect(float limit, Character *target = NULL, Vector3 _offsetFromCharacter = Vector3(0,0,0));
  void update(float deltaTime);
};


#endif
