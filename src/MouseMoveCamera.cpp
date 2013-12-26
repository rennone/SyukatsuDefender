#include "MouseMoveCamera.h"
#include <algorithm>

MouseMoveCamera::MouseMoveCamera(SyukatsuGame *game, float _frustumNear, float _frustumFar, float _frustumFOVY)
  :Camera3D(game->getWindow(), _frustumNear, _frustumFar, _frustumFOVY)
  ,syukatsuGame(game)
{  
  theta = phi = 10*Vector3::TO_RADIANS;
  distance = (_frustumFar + _frustumNear)*0.5;
  setPosition(Vector3(distance*cos(phi)*cos(theta), distance*sin(phi) , distance*cos(phi)*sin(theta) ));
}

void MouseMoveCamera::mouseTrack()
{
  static float baseX = 0;
  static float baseY = 0;

  auto input = syukatsuGame->getInput();  
  auto event = input->getMouseEvent();  
  
  Vector2 viewPos = getViewportPosition();
  float dx = (event->x - viewPos.x)/(float)getViewportWidth();
  float dy = (event->y - viewPos.y)/(float)getViewportHeight();  

  if(event->action == GLFW_PRESS || event->action == GLFW_RELEASE)
  {
    baseX = dx;
    baseY = dy;
    return;    
  }  
  
  if(event->button == GLFW_MOUSE_BUTTON_RIGHT)
  {    
    theta += (dx-baseX)*2*M_PI;
    phi   += (dy-baseY)*2*M_PI;
    phi = min(80*Vector3::TO_RADIANS, max(10*Vector3::TO_RADIANS, phi));    
    setPosition(Vector3(distance*cos(phi)*cos(theta), distance*sin(phi) , distance*cos(phi)*sin(theta) ) + getLook());
  }
  else if(event->button == GLFW_MOUSE_BUTTON_LEFT)  
    translate(-(dx-baseX)*500, (dy-baseY)*500, 0);
  

  if(input->getKeyState(GLFW_KEY_LEFT) == GLFW_PRESS)      
    translate(100, 0, 0);      
  else if(input->getKeyState(GLFW_KEY_RIGHT) == GLFW_PRESS)  
    translate(-100,0,0);
  if(input->getKeyState(GLFW_KEY_UP) == GLFW_PRESS)
    translate(0,100,0);
  else if(input->getKeyState(GLFW_KEY_DOWN) == GLFW_PRESS)
    translate(0,-100,0);  
  
  baseX = dx;
  baseY = dy;
}

void MouseMoveCamera::translate(float dx, float dy, float dz)
{
  Vector3 axisZ = getLook() - getPosition();
  Vector3 axisX = axisZ.cross(getUp());
  axisX.normalize();

  Vector3 axisY = axisX.cross(axisZ);
  axisY.normalize();

  Vector3 move = axisX*dx + axisY*dy + axisZ*dz; 
  setLook( getLook() + move);
  setPosition(getPosition() + move);    
}

void MouseMoveCamera::setViewportAndMatricesWithMouse()
{
  mouseTrack();
  setViewportAndMatrices();
}
