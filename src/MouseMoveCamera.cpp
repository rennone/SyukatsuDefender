#include "MouseMoveCamera.h"
#include "Debugger.h"
#include "Field.h"

#include <algorithm>

static const int size = Field::cellSize*Field::cellNum;
static const Vector2 center(size/2, size/2);
static const int range = size/2;

static bool inRegion(Vector2 p, float delta = 0)
{
  return center.x-range-delta < p.x &&  p.x < center.x+range+delta &&  center.y-range-delta < p.y && p.y < center.y+range+delta;
}

//pos1, dir1 の直線と, pos2, dir2の直線との交差判定, cPosは衝突位置, tには dir2の方の衝突時の時間が入る
static bool crossLines2D(const Vector2 &pos1, const Vector2 &dir1, const Vector2 &pos2, const Vector2 &dir2, float &t, Vector2 &cPos)
{
  if( dir1.x == dir2.x && dir1.y == dir2.y)
    return false;

  t = (dir1.y*pos1.x - dir1.x*pos1.y + dir1.x*pos2.y - dir1.y*pos2.x) / (dir1.y*dir2.x - dir1.x*dir2.y);
  cPos = pos2 + dir2*t;
  return true;  
}

static bool inField(Vector3 afterLook, Vector3 beforeLook, Vector3 &collision)
{
  Vector2 af( afterLook.x,  afterLook.z);
   //afterが円の中にある時は問題ない
  if ( center.distSquared(af) < range*range )
    return true;

  Vector2 bf(beforeLook.x, beforeLook.z);  
  Vector2 dir = af - bf;

  float a = dir.x*dir.x + dir.y*dir.y;
  float b = dir.dot(bf-center);  
  float c = bf.distSquared(center) - range*range;
  
  float D = b*b - a*c;  
  //衝突しないのは beforeが円の外にあるときだけなので, リセット
  if( D < 0)
  {
    collision.set(center.x, 0,center.y);
    return false;
  }
  
  float t1 = (-b + sqrt(D))/a;
  float t2 = (-b - sqrt(D))/a;

  //2つの交点がどちらも, 前, もしくは後ろにあるのは beforeが円の外にあるときだけなので, リセット
  if( (t1>0 && t2>0) || (t1<0 && t2<0) )
  {
    collision.set(center.x, 0,center.y);
    return false;
  }

  //正の方をとってくる
  float t = max(t1,t2);
  auto cPos = bf + dir*t;
  auto normal = center - cPos;
  normal.normalize();
  cPos += normal*3;
  collision.set(cPos.x, 0, cPos.y);
  return false;
  
/*
  Vector2 dir(move.x, move.z);
  if( inRegion(af) )
    return true;

  const Vector2 nearRight(center.x-range, center.y-range);
  const Vector2 nearLeft (center.x+range, center.y-range);
  const Vector2 farLeft  (center.x+range, center.y+range);
  const Vector2 farRight (center.x-range, center.y+range);

  Vector2 edge[4] = {nearRight, nearLeft, farLeft, farRight };  
  
  for(int i=0; i<4; i++)
  {
    float t;
    Vector2 cPos;
    
    //4辺との交点を計算
    if ( !crossLines2D(edge[i], edge[(i+1)%4]-edge[i], bf, dir, t, cPos) )
      continue;

    if (t<0)
      continue;

    float epsilon = 0.1;
    if(inRegion(cPos, epsilon))
    {
      
      Vector2 normal = edge[(i+2)%4] - edge[(i+1)%4];;
      if ( edge[i].distanceTo(cPos) < epsilon )
      {
        normal += edge[(i+1)%4] - edge[i];
      }
      else if( edge[(i+1)%4].distanceTo(cPos) < epsilon )
      {
        normal += edge[(i+3)%4] - edge[(i+2)%4];
      }

      normal.normalize();
      auto col = cPos + normal*3;
      collision.set(col.x, 0, col.y);
      return false;
    }
  }

  //ここにくるのは beforeが外にあるとき=>バグってるときなので, 強制的にもどす
  collision.set(center.x, 0,center.y);
  return false;
*/
}


MouseMoveCamera::MouseMoveCamera(SyukatsuGame *game, float _frustumNear, float _frustumFar, float _frustumFOVY)
  :Camera3D(game->getWindow(), _frustumNear, _frustumFar, _frustumFOVY)
  ,syukatsuGame(game)
  ,nearDistance(200)
  ,farDistance(1000)
  ,lowAngle(30)
  ,highAngle(80)
{
  setLook(Vector3(size/2, 0, size/2));
  
  theta = phi = 45*Vector3::TO_RADIANS;
  
  distance = (nearDistance + farDistance)*0.5;
  setPosition(getLook()+Vector3(distance*cos(phi)*cos(theta), distance*sin(phi) , distance*cos(phi)*sin(theta) ));
}

void MouseMoveCamera::mouseTrack()
{
  checkMouse();
  checkScroll();
  checkKeyboard();
  Debugger::drawDebugInfo("MouseMoveCamera.cpp", "distance", distance);
}

void MouseMoveCamera::checkKeyboard()
{
  auto input = syukatsuGame->getInput();

  const float move = 10;
  if(input->getKeyState(GLFW_KEY_LEFT) == GLFW_REPEAT)      
    translate(-move, 0, 0);      
  else if(input->getKeyState(GLFW_KEY_RIGHT) == GLFW_REPEAT)  
    translate(move,0,0);
  if(input->getKeyState(GLFW_KEY_UP) == GLFW_REPEAT)
    translate(0,0,move);
  else if(input->getKeyState(GLFW_KEY_DOWN) == GLFW_REPEAT)
    translate(0,0,-move);  
}

//スクロールチェック
void MouseMoveCamera::checkScroll()
{
  auto scroll = syukatsuGame->getInput()->getScrollEvent();
  Debugger::drawDebugInfo("MouseMoveCamera.cpp", "scroll", Vector2(scroll->offsetX, scroll->offsetY));
  
  if(scroll->offsetY !=0)
  {
    distance -= scroll->offsetY*scrollrate;
    distance = min(farDistance, max(nearDistance, distance));
    setPosition(Vector3(distance*cos(phi)*cos(theta), distance*sin(phi) , distance*cos(phi)*sin(theta) ) + getLook());
  }  
}

void MouseMoveCamera::checkMouse()
{
  static float baseX = 0;
  static float baseY = 0;
  auto event = syukatsuGame->getInput()->getMouseEvent();
  
  Vector2 viewPos = getViewportPosition();
  float dx = (event->x - viewPos.x)/(float)getViewportWidth();
  float dy = (event->y - viewPos.y)/(float)getViewportHeight();  
    
  if(event->action == GLFW_PRESS || event->action == GLFW_RELEASE)
  {
    baseX = dx;
    baseY = dy;
    return;    
  }  

  const float speed = 500;
  if(event->button == GLFW_MOUSE_BUTTON_RIGHT || (event->button == GLFW_MOUSE_BUTTON_LEFT && event->modifier == GLFW_MOD_SHIFT))
  {    
    theta += (dx-baseX)*2*M_PI;
    
    if(theta>2*M_PI)
      theta-=2*M_PI;
    else if(theta<0)
      theta+=2*M_PI;
    
    phi   += (dy-baseY)*2*M_PI;
    phi = min(highAngle*Vector3::TO_RADIANS, max(lowAngle*Vector3::TO_RADIANS, phi));
    setPosition(Vector3(distance*cos(phi)*cos(theta), distance*sin(phi) , distance*cos(phi)*sin(theta) ) + getLook());
  }
  else if( event->button == GLFW_MOUSE_BUTTON_LEFT)
    translate(-(dx-baseX)*speed, (dy-baseY)*speed, 0); 
  
  baseX = dx;
  baseY = dy;
}

void MouseMoveCamera::translate(float dx, float dy, float dz)
{
  Vector3 axisZ = getLook() - getPosition();
  axisZ.normalize();
  
  Vector3 axisX = axisZ.cross(getUp());
  axisX.normalize();

  Vector3 axisY = axisX.cross(axisZ);
  axisY.normalize();

  axisX.y = axisZ.y = 0;
  
  axisX.normalize();
  axisZ.normalize();
  
  Vector3 move = axisX*dx + axisY*dy + axisZ*dz;

  Vector3 collision;
  auto afterLook = getLook()+move;
  auto afterPos  = getPosition()+move;
  auto direction = afterLook - afterPos;
  
  if ( direction.y == 0 )
  {
    //あってはならないので, 本来ならエラー処理
    return;
  }
  
  double t = -afterPos.y/direction.y;
  afterLook = afterPos + direction*t;
  
  if ( inField( afterLook, getLook(), collision) )
  {
    setLook( getLook() + move);
  }
  else
  {
    auto move = collision - getLook();
    setLook( collision );
  }
  setPosition( getLook() +  Vector3(distance*cos(phi)*cos(theta), distance*sin(phi) , distance*cos(phi)*sin(theta)) );
}

void MouseMoveCamera::setViewportAndMatricesWithMouse()
{
//  mouseTrack();
  setViewportAndMatrices();
}

Vector3 MouseMoveCamera::screenToWorldRetina(const Vector3 &touch)
{
  
  int height=960;           

  Vector2 viewPos = getViewportPosition();
  
  //view内が-0.5~0.5の範囲になる様変換, 左下(-0.5, -0.5)
  float screenX = (       touch.x - viewPos.x )/(float)getViewportWidth();
  float screenY = (height-touch.y - viewPos.y )/(float)getViewportHeight();
    
  float ratio = getViewportWidth()/(float)getViewportHeight();

  //near平面における画面のサイズ
  float nearHeight = 2*1*tan(0.5*45*Vector3::TO_RADIANS);
  float nearWidth  = nearHeight*ratio;

  auto axisZ = getLook() - getPosition();           //カメラを中心としたZ軸
  axisZ.normalize(); 
  Vector3 axisY = getUp() - axisZ.dot(getUp())*axisZ; //screenYに対応 Y軸
  axisY.normalize();
  Vector3 axisX = axisZ.cross(axisY);       //screenXに対応 X軸
  axisX.normalize();  
  Vector3 direction = axisY*screenY*nearHeight + axisX*screenX*nearWidth + axisZ*1;  
  direction.normalize();
  
  return direction;  
}

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

void MouseMoveCamera::zoom(float amount)
{
  distance = min(farDistance, max(nearDistance, distance+amount));
}
  
/*
  //中心に近い位置にLookをもってるくる
  auto dir = getLook() - getPosition();
  
  if(dir.length() == 0)
    return;
  
  float fieldSize = Field::cellNum*Field::cellSize/2.0;  
  float t =  dir.dot(Vector3(fieldSize, 0, fieldSize)-getPosition() )/(dir.x*dir.x + dir.y*dir.y + dir.z*dir.z);
  if(t<= 0)
    return;
  
  Vector3 newLook = getPosition() + dir*t;
  if(newLook.y < 0)
  {
    float u = -newLook.y/dir.y;
    newLook += dir*u;
  }  
  setLook(newLook);
*/
