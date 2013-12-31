#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include <string>
#include "Widget.h"
#include "Rectangle.h"

class TextBox:public Widget<string>
{
  Rectangle region;
  int fontSize;  
public:
  TextBox(string text);  
  TextBox(string text, Vector2 lowerLeft, Vector2 size, int fontSize);  
  ~TextBox() { }
  void render(bool focus, SpriteBatcher *batcher);  
};

#endif
