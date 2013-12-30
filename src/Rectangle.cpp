#include "Rectangle.h"
#include <math/Vector2.h>
Rectangle::Rectangle(float left, float lower, float wid, float hei)
  :lowerLeft(Vector2(left, lower))
  ,size(Vector2(wid, hei))
{  
}

Rectangle::Rectangle(Vector2 _lowerLeft, Vector2 _size)
  :lowerLeft(_lowerLeft)
  ,size(_size)
{  
}

inline bool Rectangle::inRegion(const float &x,const float &y) const
{
  return lowerLeft.x < x && x < lowerLeft.x+size.x && lowerLeft.y < y && y < lowerLeft+size.y;  
}

inline bool Rectangle::inRegion(const Vector2 &point)
{
  return inRegion(point.x, point.y)
}



