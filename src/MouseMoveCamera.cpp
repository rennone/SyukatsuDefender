#include "MouseMoveCamera.h"
#include "Debugger.h"
#include "Field.h"

#include <algorithm>

MouseMoveCamera::MouseMoveCamera(SyukatsuGame *game, float _frustumNear, float _frustumFar, float _frustumFOVY)
  :Camera3D(game->getWindow(), _frustumNear, _frustumFar, _frustumFOVY)
  ,syukatsuGame(game)
  ,nearDistance(_frustumNear+100)
  ,farDistance(_frustumFar*0.5)
  ,lowAngle(10)
  ,highAngle(70)
{  
  theta = phi = 10*Vector3::TO_RADIANS;
  distance = (nearDistance + farDistance)*0.5;
  setPosition(Vector3(distance*cos(phi)*cos(theta), distance*sin(phi) , distance*cos(phi)*sin(theta) ));
}

void MouseMoveCamera::mouseTrack()
{
  
}

void MouseMoveCamera::checkKeyboard()
{
  auto input = syukatsuGame->getInput();
  
  if(input->getKeyState(GLFW_KEY_LEFT) == GLFW_REPEAT)      
    translate(-10, 0, 0);      
  else if(input->getKeyState(GLFW_KEY_RIGHT) == GLFW_REPEAT)  
    translate(10,0,0);
  if(input->getKeyState(GLFW_KEY_UP) == GLFW_REPEAT)
    translate(0,0,10);
  else if(input->getKeyState(GLFW_KEY_DOWN) == GLFW_REPEAT)
    translate(0,0,-10);  
}

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
  
  if(event->button == GLFW_MOUSE_BUTTON_RIGHT || (event->button == GLFW_MOUSE_BUTTON_LEFT && event->modifier == GLFW_MOD_SHIFT))
  {    
    theta += (dx-baseX)*2*M_PI;
    phi   += (dy-baseY)*2*M_PI;
    phi = min(highAngle*Vector3::TO_RADIANS, max(lowAngle*Vector3::TO_RADIANS, phi));    
    setPosition(Vector3(distance*cos(phi)*cos(theta), distance*sin(phi) , distance*cos(phi)*sin(theta) ) + getLook());
  }

  if(event->button == GLFW_MOUSE_BUTTON_LEFT)
    translate(-(dx-baseX)*500, (dy-baseY)*500, 0); 
  
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
  setLook( getLook() + move);
  setPosition(getPosition() + move);


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
}

void MouseMoveCamera::setViewportAndMatricesWithMouse()
{
  checkMouse();
  checkScroll();
  checkKeyboard();  
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
