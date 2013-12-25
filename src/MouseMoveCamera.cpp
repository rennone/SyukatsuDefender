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
  
  if(event->button == GLFW_MOUSE_BUTTON_LEFT)
  {    
    theta += (dx-baseX)*2*M_PI;
    phi   += (dy-baseY)*2*M_PI;
    phi = min(80*Vector3::TO_RADIANS, max(10*Vector3::TO_RADIANS, phi));    
    baseX = dx;
    baseY = dy;
  }

  setPosition(Vector3(distance*cos(phi)*cos(theta), distance*sin(phi) , distance*cos(phi)*sin(theta) ));
}

void MouseMoveCamera::setViewportAndMatricesWithMouse()
{
  mouseTrack();
  setViewportAndMatrices();  
}


