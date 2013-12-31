#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <math/Vector2.h>

class Rectangle
{
public:
  Vector2 lowerLeft;
  Vector2 size;
  Rectangle() {  }
  Rectangle(float left, float lower, float width, float height);
  Rectangle(Vector2 lowerLeft, Vector2 size);
  ~Rectangle() {  }
  bool inRegion(const float &x,const float &y) const;
  bool inRegion(const Vector2 &point) const;  
};


#endif
