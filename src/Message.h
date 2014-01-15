#ifndef MESSAGE_H
#define MESSAGE_H

#include <math/Vector3.h>
#include <string>

#include "Actor.h"

class Message : public Actor
{
  Vector3 position;
  float limitTime;  //消えるまでの時間
  bool isFixed;     //位置を固定するかどうか
  string text;
  float elapsedTime;

  Vector3 offset;  //positionを文字の中心にする為のオフセット
public:
  Message(string text, Vector3 position, float limit, bool isFixed);
  ~Message();

  void setMessage(string text, Vector3 position, float limit, bool isFixed);
  
  void setPosition(Vector3 pos)
  {
    position = pos;    
  }

  Vector3 getPosition() const
  {
    return position;    
  }
  
  void update(float deltaTime);
  void render(float deltaTime);
  void render(const Vector3 &cameraPos, float deltaTime);
};
#endif
