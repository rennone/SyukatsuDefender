#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAFER_H

#include <math/Vector3.h>
#include "Information.h"
#include <syukatsu/syukatsu.h>
#include "Message.h"

using namespace TextColors;

class Character;
class MessageManager
{
public:
  //メッセージの描画, 毎フレーム消える 2D
  void drawMessage(string text, Vector2 point ,TextColor color=RedText);
  
  //メッセージの描画, 毎フレーム消える 3D
  void drawMessage(string text, Vector3 position, TextColor color=RedText);
  
  void effectMessage(string text, Vector2 point, float limit = 1,TextColor color=RedText);
  
  //エフェクトとしてのメッセージ, limitTimeで消える.
  void effectMessage(string text, Vector3 position, float limit = 1,TextColor color=RedText);
  
  //Characterについているエフェクトメッセージ, offsetFromCharacterはキャラクタからのオフセット量
  void effectMessage(string text, Character *target, float limit = 1,TextColor color=RedText, Vector3 offsetFromCharacter = Vector3(0,0,0));
  
  //3Dメッセージの描画
  void render3DMessage(float deltaTime, Vector3 cameraPos, Vector3 cameraLook);

  //2Dメッセージの描画
  void render2DMessage(float deltaTime);
  
  //3Dメッセージを2Dスクリーン上に投影描画(todo 未実装)
  void render3DMessageIn2DScreen(float deltaTime, Camera3D *camera, Camera2D *camera2);
  
  void update(float deltaTime);

  //全てのメッセージをNoUseにする
  void reset();
  
  //バッファにためずに, 即座に描画する.
  //bitmapFontを使っているので英字のみだが, テクスチャとして描画できる
  static void drawBitmapString(string str, Vector2 point, float size, TextColor color=RedText );  
  static void drawBitmapString(string str, Vector3 position, Vector3 normal, float size, TextColor color=RedText, int rotateDegree=0 );
  static MessageManager *getInstance();  
private:

  int msgIndex, effectMsgIndex;  //バッファインデックスの位置
  
  Message       *getNewMessage();
  EffectMessage *getNewEffectMessage();   
  const int maxMessage;     //メッセージの数
  
  //ポインタの配列を動的に確保する為にダブルポインタ
  Message **instantMessages;      //毎フレームリセットされる,メッセージ
  EffectMessage **effectMessages; //一度設定したら, 指定した時間まで残るメッセージ
  
   MessageManager(int maxMessage);
  ~MessageManager();
  MessageManager& operator=(const MessageManager&) const;
};

#endif
