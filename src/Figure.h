#ifndef FIGURE_H
#define FIGURE_H
/*
#include <math/Vector2.h>
#include <math/Vector3.h>

//図形インターフェース
class Figure
{
public:
  Figure()
  {
  }

  //図形内にあるかどうか
//  virtual bool inRegion()

};

class Line2D;
class Rectangle2D;
class Triangle2D;

class Line2D : public Figure
{
public:
//始点と方向ベクトル
  Vector2 p, d;
Line2D(Vector2 _p, Vector2 _d)
  :p(_p), d(_d)
  {
    p.normalize();
    d.normalize();
  }
  ~Line2D() {}

  bool inRegion(Vector2 v) const
  {
    auto dir = v-p;
    return d.dot(dir) == dir.length();
  }

  bool crossLine(const Line2D &other, Vector2 &crossPoint);
  bool crossRect(const Rectangle2D &other, Vector2 &crossPoint);
  bool crossTri(const Triangle2D &other, Vector2 &crossPoint);
};

class Rectangle2D : public Figure
{
public:
//中心とサイズ
  Vector2 center, size;

Rectangle2D(Vector2 c, Vector2 s)
    :center(c), size(s)
  {
  }
  
  ~Rectangle2D()
  {
  }

  bool inRegion(const Vector2 &v) const
  {
    return center.x-size.x/2 <= v.x && v.x <= center.x-size.x/2  && center.y-size.y/2 <= v.y && v.y <= center.y+size.y/2;
  }
  bool crossLine(const Line2D &other, Vector2 &crossPoint);
  bool crossRect(const Rectangle2D &other, Vector2 &crossPoint);
  bool crossTri(const Triangle2D &other, Vector2 &crossPoint);
};

class Triangle2D : public Figure
{
public:
  Vector2 v1,v2,v3;

  Triangle2D(Vector2 _v1, Vector2 _v2, Vector2 _v3)
    :v1(_v1), v2(_v2), v3(_v3)
  {    
  }

  ~Triangle2D()
  {
  }
  

  bool inRegion(const Vector2 &v) const
  {    
    const auto A = v-v1;
    const auto B = v-v2;
    const auto C = v-v3;

    const auto P = v2-v1;
    const auto Q = v3-v2;
    const auto R = v1-v3;

    const float z1 = A.cross(P);
    const float z2 = B.cross(Q);
    const float z3 = C.cross(R);

    return (z1>=0 && z2>=0 && z3>=0) || (z1<=0 && z2<=0 && z3<=0);
  }
  
  bool crossLine(const Line2D &other, Vector2 &crossPoint);
  bool crossRect(const Rectangle2D &other, Vector2 &crossPoint);
  bool crossTri(const Triangle2D &other, Vector2 &crossPoint);
};

class Rectangle3D
{
public:
//中心, サイズ, 法線(z軸), 上方向(y軸を決める基準ベクトル, 普通は(0,1,0))ベクトル, deg回転角度
  Vector3 center, size;
  Vector3 normal, up;
  int deg;
public :
  Rectangle3D(Vector3 c, Vector3 s, Vector3 n = Vector3(0,0,1))
    :center(c), size(s), normal(n), deg(0)
  {
    normal.normalize();  

    if ( abs(normal.y) == 1 )
    {
      //法線が上を向いている時は, upはz軸を向いている
      up.set(0,0,normal.y);
    }
    else
    {
      //それ以外はy軸に一番近いベクトル
      up.set(0,1,0);
      up = up - up.dot(normal)*normal;
    }
  }

  ~Rectangle3D()
  {
  }

};

class Line3D
{
public:
  Vector3 p,d;
  Line3D(Vector3 _p, Vector3 _d)
    :p(_p),d(_d)
  {
    p.normalize();
    d.normalize();
  }
  
  ~Line3D()
  {
  }

};


class Triangle3D
{
public:
//頂点と法線ベクトル
  Vector3 v1,v2,v3;
  Vector3 normal;

  // front : 表面がどちらを向いているか
Triangle3D(Vector2 _v1, Vector2 _v2, Vector2 _v3, Vector3 front = Vector3(0,1,0))
    :v1(_v1), v2(_v2), v3(_v3)
  {
    normal = (v2-v1).cross(v3-v2);    
    normal.normalize();

    if ( front.dot(normal) < 0)
      normal = -normal;
  }

  ~Triangle3D()
  {
  }

  //法線ベクトルを反転
  void reverse()
  {
    normal = -normal;
  }
  

  bool inRegion(Vector3 v) const
  {
    //頂点からの方向ベクトルが法線と直交していなければ面上にはない
    const float epsilon = 1.0e-5; //誤差を許容する範囲
    const auto u = v-v1;
    if( abs(u.dot(normal)) > epsilon )
      return false;

    // vが平面上にある時
    const auto A = v-v1;
    const auto B = v-v2;
    const auto C = v-v3;

    const auto P = v2-v1;
    const auto Q = v3-v2;
    const auto R = v1-v3;

    const auto z1 = A.cross(P);
    const auto z2 = B.cross(Q);
    const auto z3 = C.cross(R);

    return z1.dot(z2)>=0 && z2.dot(z3)>=0 && z3.dot(z1)>=0;    
  }

};


class Cube
{
public:
  Vector3 center, size;
  //normal => size.xとsize.y面の向く方向
  Vector3 normal, up;
  int deg;

Cube(Vector3 c, Vector3 s)
  :center(c), size(s)
  {
//    n.normalize();
    if ( abs(normal.y) == 1 )
    {
      //法線が上を向いている時は, upはz軸を向いている
      up.set(0,0,normal.y);
    }
    else
    {
      //それ以外はy軸に一番近いベクトル
      up.set(0,1,0);
      up = up - up.dot(normal)*normal;
    }    
  }

  ~Cube()
  {
  }
  
  bool inRegion(const Vector3 &v) const
  {
    
  }
};
*/
#endif
