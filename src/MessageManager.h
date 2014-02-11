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
  static constexpr int maxMessage = 100;

  //メッセージの描画, 毎フレーム消える 3D
//  static void drawMessage(string text, Vector2 point , float alpha = 1, TextColor color=RedText)
  static void drawMessage(string text, Vector2 point ,TextColor color=RedText)
  {
    getInstance()->_drawMessage(text, point, color);
  }

  //メッセージの描画, 毎フレーム消える 2D
  static void drawMessage(string text, Vector3 position, TextColor color=RedText)
  {
    getInstance()->_drawMessage(text, position, color);
  }
  
  //エフェクトとしてのメッセージ, limitTimeで消える.
  static void effectMessage(string text, Vector3 position, float limit = 1,TextColor color=RedText)
  {
    getInstance()->_effectMessage(text, position, limit, color);
  }

  //Characterについているエフェクトメッセージ, offsetFromCharacterはキャラクタからのオフセット量
  static void effectMessage(string text, Character *target, float limit = 1,TextColor color=RedText, Vector3 offsetFromCharacter = Vector3(0,0,0))
  {
    getInstance()->_effectMessage(text, target, limit, color, offsetFromCharacter);
  }

  //3Dメッセージの描画
  static void render3DMessage(float deltaTime, Vector3 cameraPos)
  {
    getInstance()->_render3DMessage(deltaTime, cameraPos);    
  }

  //2Dメッセージの描画
  static void render2DMessage(float deltaTime)
  {
    getInstance()->_render2DMessage(deltaTime);
  }

  //3Dメッセージを2Dスクリーン上に投影描画(todo 未実装)
  static void render3DMessageIn2DScreen(float deltaTime, Camera3D *camera, Camera2D *camera2)
  {
    getInstance()->_render3DMessageIn2DScreen(deltaTime, camera, camera2);
  }

  static void update(float deltaTime)
  {
    getInstance()->_update(deltaTime);
  }

  //全てのメッセージをNoUseにする
  static void reset()
  {
    getInstance()->_reset();
  }

  //バッファにためずに, 即座に描画する.
  //bitmapFontを使っているので英字のみだが, テクスチャとして描画できる
  static void drawBitmapString(string str, Vector2 point, float size, TextColor color=RedText );  
  static void drawBitmapString(string str, Vector3 position, Vector3 normal, float size, TextColor color=RedText, int rotateDegree=0 );
  
private:
  static MessageManager *getInstance();
  void _render3DMessage(float deltaTime, Vector3 cameraPos);
  void _render2DMessage(float deltaTime);
  void _render3DMessageIn2DScreen(float deltaTime, Camera3D *camera, Camera2D *camera2);  
  void _update(float deltaTime);

  void _drawMessage(string text, Vector2 point    ,  TextColor color);
  void _drawMessage(string text, Vector3 position ,  TextColor color);
  void _effectMessage(string text, Vector3 position , float limit, TextColor color);
  void _effectMessage(string text, Character *target, float limit, TextColor color, Vector3 offsetFromCharacter);

  void _reset();
  int msgIndex, effectMsgIndex;
  Message       *getNewMessage();
  EffectMessage *getNewEffectMessage(); 
  Message       *instantMessages[maxMessage];  //毎フレームリセットされる,メッセージ
  EffectMessage  *effectMessages[maxMessage];  //一度設定したら, 指定した時間まで残るメッセージ  
  
   MessageManager();
  ~MessageManager();
  MessageManager& operator=(const MessageManager&) const;
};

#endif
