#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAFER_H

#include <math/Vector3.h>
#include "Information.h"
#include <syukatsu/syukatsu.h>

//class Message;
//class EffectMessage;
class Character;
#include "Message.h"
class MessageManager
{
public:
  static void initialize();
  static void drawMessage(string text, Vector3 position, float alpha = 1,TextColors::TextColors textColor=TextColors::RED);

  //エフェクトとしてのメッセージ, limitTimeで消える.
  static void effectMessage(string text, Vector3 position, float limit, TextColors::TextColors textColor=TextColors::RED);
  
  static void effectMessage(string text, Character *target, float limit,
                            TextColors::TextColors textColor=TextColors::RED, Vector3 offsetFromCharacter = Vector3(0,0,0));
  
  static void render(float deltaTime, Vector3 cameraPos);
  static void update(float deltaTime);
  static void render2(float deltaTime, Camera3D *camera, Camera2D *camera2);

//  static void setEffectMessagePosition(int id, const Vector3 position);
private:
  static constexpr int maxMessage = 100;
  static int msgIndex, effectMsgIndex;
  
  static Message       *instantMessages[maxMessage];  //毎フレームリセットされる,メッセージ
  static EffectMessage *effectMessages[maxMessage];    //一度設定したら, 指定した時間まで残るメッセージ
  
  static TextColor textColors[TextColors::COLORNUM];

  MessageManager(){};
  ~MessageManager(){};
  MessageManager& operator=(const MessageManager&) const;
};

#endif
