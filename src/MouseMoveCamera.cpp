#include "MouseMoveCamera.h"

MouseMoveCamera::MouseMoveCamera(SyukatsuGame *game, float _frustumNear, float _frustumFar, float _frustumFOVY)
  :Camera3D(game->getWindow(), _frustumNear, _frustumFar, _frustumFOVY)
  ,syukatsuGame(game)
{
  
  theta = phi = 0;
  distance = (_frustumFar + _frustumNear)*0.5;
}

void MouseMoveCamera::mouseTrack()
{
  auto event = syukatsuGame->getInput()->getMouseEvent();
  Vector2 viewPos = getViewportPosition();
  
  theta = (event->x - viewPos.x)/(float)getViewportWidth()*2*M_PI;
  phi   = (event->y - viewPos.y)/(float)getViewportHeight()*2*M_PI;  
  
  setPosition(Vector3(distance*cos(phi)*cos(theta), distance*sin(phi) , distance*cos(phi)*sin(theta) ));  
}

void MouseMoveCamera::setViewportAndMatricesWithMouse()
{
  mouseTrack();
  setViewportAndMatrices();  
}


