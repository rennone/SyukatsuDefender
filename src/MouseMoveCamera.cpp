#include "MouseMoveCamera.h"
#include "Debugger.h"
#include "Field.h"

#include <algorithm>


//フィールドのサイズ
static const int fieldSize = Field::cellSize*Field::cellNum;
//フィールドの中心
static const Vector2 center(fieldSize/2, fieldSize/2);
//カメラの移動可能半径
static const int range = fieldSize/sqrt(2.0);

//centerを中心とした,半径rangeの円の外にでないようにする
//戻り値 : フィールド内にあるか否か
//collision : 衝突位置
static bool inField(Vector3 afterLook, Vector3 beforeLook, Vector3 &collision)
{
  Vector2 af( afterLook.x,  afterLook.z);
  
   //afterが円の中にある時は問題ない
  if ( center.distSquared(af) < range*range )
    return true;
  
  //外にでるときは, 衝突位置を検出
  Vector2 bf(beforeLook.x, beforeLook.z);  
  Vector2 dir = af - bf;

  float a = dir.x*dir.x + dir.y*dir.y;
  float b = dir.dot(bf-center);  
  float c = bf.distSquared(center) - range*range;
  
  float D = b*b - a*c;
  
  if( D < 0)
  {
    //衝突しないのは beforeが円の外にあるときなので, リセット
    collision.set(center.x, 0,center.y);
    return false;
  }
  
  float t1 = (-b + sqrt(D))/a;
  float t2 = (-b - sqrt(D))/a;

  if( (t1>0 && t2>0) || (t1<0 && t2<0) )
  {
    //2つの交点がどちらも, 前, もしくは後ろにあるのは beforeが円の外にあるときだけなので, リセット
    collision.set(center.x, 0, center.y);
    return false;
  }

  float t = max(t1,t2);  //正(前)の方をとってくる
  auto cPos = bf + dir*t; //衝突位置を検出

   //法線ベクトルを計算
  auto normal = center - cPos;
  normal.normalize();
  
  cPos += normal*3; //円の上でなく, 少し内部にずらす
  collision.set(cPos.x, 0, cPos.y);
  return false;
}


MouseMoveCamera::MouseMoveCamera(SyukatsuGame *game, float _frustumNear, float _frustumFar, float _frustumFOVY)
  :Camera3D(game->getWindow(), _frustumNear, _frustumFar, _frustumFOVY)
  ,syukatsuGame(game)
  ,nearDistance(200)
  ,farDistance(600)
  ,lowAngle(30)
  ,highAngle(80)
{
  setLook(Vector3(fieldSize/2, 0, fieldSize/2));
  
  theta = phi = 45*Vector3::TO_RADIANS;  
  distance = (nearDistance + farDistance)*0.5;
  setPosition(getLook()+Vector3(distance*cos(phi)*cos(theta), distance*sin(phi) , distance*cos(phi)*sin(theta) ));
}

//マウス(とキーボード)の動きをチェック
void MouseMoveCamera::mouseTrack(float deltaTime)
{
  checkMouse();
  checkScroll();
  checkKeyboard(deltaTime);
}

//キーボードチェック
//平行移動
void MouseMoveCamera::checkKeyboard(float deltaTime)
{
  auto input = syukatsuGame->getInput();

  const float speed = fieldSize;
  const float move = speed*deltaTime;
  
  if(input->getKeyState(GLFW_KEY_LEFT) == GLFW_REPEAT)      
    translate(-move, 0, 0, true);      
  else if(input->getKeyState(GLFW_KEY_RIGHT) == GLFW_REPEAT)  
    translate(move,0,0, true);
  if(input->getKeyState(GLFW_KEY_UP) == GLFW_REPEAT)
    translate(0,0,move, true);
  else if(input->getKeyState(GLFW_KEY_DOWN) == GLFW_REPEAT)
    translate(0,0,-move, true);
}

//スクロールチェック
//zoomする
void MouseMoveCamera::checkScroll()
{
  auto scroll = syukatsuGame->getInput()->getScrollEvent();
  
  if(scroll->offsetY !=0)
    zoom(-scroll->offsetY*scrollrate);
}

void MouseMoveCamera::checkMouse()
{
  //前回のマウスの位置
  static float baseX = 0;
  static float baseY = 0;
  
  auto event = syukatsuGame->getInput()->getMouseEvent();

  //カメラの中心からの, マウスの位置
  Vector2 viewPos = getViewportPosition();
  float dx = (event->x - viewPos.x)/(float)getViewportWidth();
  float dy = (event->y - viewPos.y)/(float)getViewportHeight();  

  //ドラッグ以外では変化しない
  if(event->action == GLFW_PRESS || event->action == GLFW_RELEASE)
  {
    baseX = dx;
    baseY = dy;
    return;    
  }

  if(event->button == GLFW_MOUSE_BUTTON_RIGHT || (event->button == GLFW_MOUSE_BUTTON_LEFT && event->modifier == GLFW_MOD_SHIFT))
  {
    //右ドラッグかShift+左ドラッグで回転
    const float speed = 2*M_PI;
    rotate( (dx-baseX)*speed, (dy-baseY)*speed);
  }
  else if( event->button == GLFW_MOUSE_BUTTON_LEFT)
  {
    //左ドラッグで移動
    const float speed = 500;
    translate(-(dx-baseX)*speed, (dy-baseY)*speed, 0); 
  }

  //今回のマウスの位置を記憶
  baseX = dx;
  baseY = dy;
}


//平行移動, 最後の引数はXZ移動のみに限定するかのフラグ(キーボードでの移動で使う)
void MouseMoveCamera::translate(float dx, float dy, float dz, bool onlyXZPlane)
{
  //マウスの見ている方向をz軸とした, 左手系の基底ベクトルを求める
  Vector3 axisZ = getLook() - getPosition();
  Vector3 axisX = axisZ.cross(getUp());
  Vector3 axisY = axisX.cross(axisZ);

  if(onlyXZPlane)
  {
    //y方向には変化しないようにする
    axisX.y = axisZ.y = axisY.y = 0;
  }
  
  axisX.normalize();
  axisY.normalize();
  axisZ.normalize();

  //移動量
  Vector3 move = axisX*dx + axisY*dy + axisZ*dz;
  
  Vector3 collision;
  auto afterLook = getLook()+move;
  auto afterPos  = getPosition()+move;
  auto direction = afterLook - afterPos;  //移動後のカメラの方向ベクトル

  if ( direction.y == 0 )
  {    
    //あってはならないので,強制的に中心に戻す
    setLook( Vector3(center.x, 0, center.y) );
    return;
  }

  //フォールド平面上との交点を見るようにする
  double t = -afterPos.y/direction.y;
  afterLook = afterPos + direction*t;

  //衝突判定
  if ( inField( afterLook, getLook(), collision) )
    setLook( afterLook );
  else
    setLook( collision );

  //位置を再計算
  setPosition( getLook() +  Vector3(distance*cos(phi)*cos(theta), distance*sin(phi) , distance*cos(phi)*sin(theta)) );
}

//今の視点を中心に回転
void MouseMoveCamera::rotate(float deltaTheta, float deltaPhi)
{
  theta+=deltaTheta;
  
  if(theta > 2*M_PI )
    theta -= 2*M_PI;
  else if(theta<0)
    theta += 2*M_PI;
  
  phi = min(highAngle*Vector3::TO_RADIANS, max(lowAngle*Vector3::TO_RADIANS, phi+deltaPhi));

  setPosition(Vector3(distance*cos(phi)*cos(theta), distance*sin(phi) , distance*cos(phi)*sin(theta) ) + getLook());
}

//ズーム
void MouseMoveCamera::zoom(float amount)
{
  distance = min(farDistance, max(nearDistance, distance+amount));
  setPosition(Vector3(distance*cos(phi)*cos(theta), distance*sin(phi) , distance*cos(phi)*sin(theta) ) + getLook());
}
