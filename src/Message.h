#ifndef MESSAGE_H
#define MESSAGE_H

#include <math/Vector3.h>
#include <string>

#include "Actor.h"

class Message : public Actor
{
  friend class MessageManager;  
  Vector3 position;
  float limitTime;  //消えるまでの時間 負だと消えない
  string text;
  float elapsedTime;
  Vector3 offset;  //positionを文字の中心にする為のオフセット
  Message();  
  ~Message();
  Message& operator=(const Message&) const;  
public:
  void setMessage(string text, Vector3 position, float limit);  
  void update(float deltaTime);
};
#endif
