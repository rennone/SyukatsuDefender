#ifndef ICON_H
#define ICON_H

#include <math/Vector2.h>
class TextureRegion;

class Icon
{
public:
  TextureRegion *image;
  Vector2 lowerLeft;
  Vector2 size;
  
  bool inRegion(const Vector2 &touch) const
  {
     return lowerLeft.x < touch.x && touch.x < lowerLeft.x+size.x && lowerLeft.y < touch.y && touch.y < lowerLeft.y + size.y;
  }
  
  Icon(const Vector2 &_lowerLeft, const Vector2 &_size, TextureRegion *_image)
    :image(_image)
    ,lowerLeft(_lowerLeft)
    ,size(_size)
  {
  }
  
  ~Icon()
  {
  }
};

#endif
