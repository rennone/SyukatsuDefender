#ifndef FIGURE_H
#define FIGURE_H

#include <math/Vector2.h>
#include <math/Vector3.h>

class Line2D
{
public:
//始点と方向ベクトル
  Vector2 p, d;
Line2D(Vector2 _p, Vector2 _d)
  :p(_p), d(_d)
  {
  }
  ~Line2D() {}
};

class Line3D
{
public:
  Vector3 p,d;
};


class Figure
{
  
};

class Triangle2D
{
public:
  Vector2 v1,v2,v3;
};

class Triangle3D
{
public:
//頂点と法線ベクトル
  Vector3 v1,v2,v3;
  Vector3 normal;
};

class Rectangle2D
{
public:
//中心とサイズ
  Vector2 center, size;
};

class Rectangle3D
{
public:
//中心, サイズ, 法線(z軸), 上方向(y軸を決める基準ベクトル, 普通は(0,1,0))ベクトル
  Vector3 center, size;
  Vector3 normal, up;
};

class Cube
{
public:
  Vector3 center, size;
};

#endif
