#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAFER_H

#include <math/Vector3.h>
#include "Information.h"
#include <syukatsu/syukatsu.h>
#include "Message.h"

class Character;
class MessageManager
{
public:
  static void initialize();
  
  static void drawMessage(string text, Vector2 point   , float alpha = 1, TextColors::TextColors textColor=TextColors::RED);
  static void drawMessage(string text, Vector3 position, float alpha = 1, TextColors::TextColors textColor=TextColors::RED);
  
  //エフェクトとしてのメッセージ, limitTimeで消える.
  static void effectMessage(string text, Vector3 position, float limit,
                            TextColors::TextColors textColor=TextColors::RED);

  //Characterについているエフェクトメッセージ, offsetFromCharacterはキャラクタからのオフセット量
  static void effectMessage(string text, Character *target, float limit,
                            TextColors::TextColors textColor=TextColors::RED, Vector3 offsetFromCharacter = Vector3(0,0,0));

  //3Dメッセージの描画
  static void render(float deltaTime, Vector3 cameraPos);

  //2Dメッセージの描画
  static void render2D(float deltaTime);
  
  static void update(float deltaTime);
  static void render2(float deltaTime, Camera3D *camera, Camera2D *camera2);
  
private:
  static constexpr int maxMessage = 100;
  static int msgIndex, effectMsgIndex, msg2DIndex;

  static Message       *instantMessages2D[maxMessage];  //毎フレームリセットされる,メッセージ
  static Message       *instantMessages[maxMessage];  //毎フレームリセットされる,メッセージ
  static EffectMessage *effectMessages[maxMessage];    //一度設定したら, 指定した時間まで残るメッセージ  
  static TextColor textColors[TextColors::COLORNUM];

  MessageManager(){};
  ~MessageManager(){};
  MessageManager& operator=(const MessageManager&) const;
};

#endif
