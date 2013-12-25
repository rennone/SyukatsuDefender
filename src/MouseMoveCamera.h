#ifndef MOUSE_MOVE_CAMERA_H_2013_12_25
#define MOUSE_MOVE_CAMERA_H_2013_12_25

#include <syukatsu/syukatsu.h>
class MouseMoveCamera : public Camera3D
{
  SyukatsuGame *syukatsuGame;  
  float theta, phi;
  float distance;
public:
  MouseMoveCamera(SyukatsuGame *game, float _frustumNear, float _frustumFar, float _frustumFOVY);
  void mouseTrack();
  void setViewportAndMatricesWithMouse();  
};


#endif
