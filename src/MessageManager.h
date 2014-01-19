#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAFER_H

#include <math/Vector3.h>
#include "Information.h"

class Message;
class Color4
{
public:
Color4(float _r, float _g, float _b, float _a)
  :r(_r), g(_g), b(_b), a(_a)
  {
  }

  float r,g,b,a;
};

#include <syukatsu/syukatsu.h>
class MessageManager
{
public:
  static void initialize();
  static void drawMessage(string text, Vector3 position, float alpha = 1,TextColors::TextColors textColor=TextColors::RED);

  //エフェクトとしてのメッセージ, limitTimeで消える.
  static void effectMessage(string text, Vector3 position, float limit = -1, TextColors::TextColors textColor=TextColors::RED);
  static void render(float deltaTime, Vector3 cameraPos);
  static void update(float deltaTime);
  static void render2(float deltaTime, Camera3D *camera, Camera2D *camera2);
private:
  static constexpr int maxMessage = 100;
  static int msgIndex;
  static Message *messages[maxMessage];  
  static Color4 textColors[TextColors::COLORNUM];

  MessageManager(){};
  ~MessageManager(){};
  MessageManager& operator=(const MessageManager&) const;
};

#endif
