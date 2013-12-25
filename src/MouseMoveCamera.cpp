#include "MouseMoveCamera.h"

MouseMoveCamera::MouseMoveCamera(SyukatsuGame *game, float _frustumNear, float _frustumFar, float _frustumFOVY)
  :Camera3D(game->getWindow(), _frustumNear, _frustumFar, _frustumFOVY)
  ,syukatsuGame(game)
{
  
  theta = phi = 0;
  distance = (_frustumFar + _frustumNear)*0.5;
}

MouseMoveCamera::mouseTrack()
{
  auto event = syukatsuGame->getInput()->getMouseEvent();
  theta = (event->x - viewportX)/(float)viewportWidth*2*M_PI;
  phi   = (event->y - viewportY)/(float)viewportHeight*2*M_PI;  
  
  setPosition(Vector3(distance*cos(phi)*cos(theta), distance*sin(phi) , distance*cos(phi)*sin(theta) ));  
}

MouseMoveCamera::setViewportAndMatricesWidthMouse()
{
  mouseTrack();
  setViewportAndMatrices();  
}


