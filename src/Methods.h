#ifndef METHODS_H

#include <map>
#include <math/Vector2.h>
#include <math/Vector3.h>

using namespace std;

namespace Methods
{

  // l1(p1,d1)の直線と, l2(p2, d2)の直線の交差判定
  // colPos : 衝突位置
  // t2     : l2の方の媒介変数t
  extern bool crossLine(const Vector2 &p1, const Vector2 &d1, const Vector2 &p2, const Vector2 &d2, Vector2 &colPos, float &t2);
  extern Vector3 normalVectorOfTriangle(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3);
}
#endif



