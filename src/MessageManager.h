#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAFER_H

#include <math/Vector3.h>
#include "Information.h"
#include <syukatsu/syukatsu.h>
#include "Message.h"
#include "Actor.h"

using namespace TextColors;

class Character;
class SpriteBatcher3D;
class MessageManager : public Actor
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
  static void drawBitmapString(const string &str, const Vector2 &point, const float &size, const TextColor &color=RedText );  
  static void drawBitmapString(const string &str, const Vector3 &position, const Vector3 &normal, const float &size, const TextColor &color=RedText, const int &rotateDegree=0 );
  static void drawFrame(const Vector2 &upperLeft,const Vector2 &size, const float &lineWidth=32);
  static MessageManager *getInstance();  
private:

  int msgIndex, effectMsgIndex;  //バッファインデックスの位置
  
  Message       *getNewMessage();
  EffectMessage *getNewEffectMessage();   
  const int maxMessage;     //メッセージの数
  
  //ポインタの配列を動的に確保する為にダブルポインタ
  Message **instantMessages;      //毎フレームリセットされる,メッセージ
  EffectMessage **effectMessages; //一度設定したら, 指定した時間まで残るメッセージ
  
  MessageManager(string name, SyukatsuGame *game, int maxMessage);
  ~MessageManager();
  MessageManager& operator=(const MessageManager&) const;

  static SpriteBatcher* getSpriteBatcher();
  static SpriteBatcher3D* getSpriteBatcher3D();
};

#endif
