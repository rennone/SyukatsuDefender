#ifndef MOUSE_MOVE_CAMERA_H_2013_12_25
#define MOUSE_MOVE_CAMERA_H_2013_12_25

#include <syukatsu/syukatsu.h>

class MouseMoveCamera : public Camera3D
{
  SyukatsuGame *syukatsuGame;

  const float nearDistance, farDistance;
  const int lowAngle, highAngle;
  static constexpr float scrollrate = 60;
  float theta, phi;
  float distance;

  void checkMouse();
  void checkScroll();
  void checkKeyboard(float deltaTime);
public:
  MouseMoveCamera(SyukatsuGame *game, float _frustumNear, float _frustumFar, float _frustumFOVY);

  void mouseTrack(float deltaTime);  //マウス(とキーボード)に追従
  
  void translate(float dx, float dy, float dz, bool onlyXZPlane=false); 
  void rotate(float theta, float phi);//回転
  void zoom(float amount); //ズーム

  bool inWindow(const Vector2 &touch) const
  {
    const auto tmp = touch - getViewportPosition();
    return
      -getViewportWidth()/2 < tmp.x &&
      tmp.x < +getViewportWidth()/2 &&
      -getViewportHeight()/2 < tmp.y &&
      tmp.y < +getViewportHeight()/2;
  }
};
#endif
